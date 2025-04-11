#include"gpio_driver.h"
#include<zephyr/logging/log.h>

LOG_MODULE_REGISTER(gpio_driver,LOG_LEVEL_DBG);





/**
 * @brief GPIO pin konfigürasyonu yapar.
 *
 * Bu fonksiyon, belirtilen GPIO pin'inin yapılandırmasını gerçekleştirir. Öncelikle GPIO portunun geçerli olup olmadığı kontrol edilir, 
 * ardından portun hazır olduğu doğrulanır. Son olarak, belirtilen bayraklarla pin yapılandırması yapılır. 
 * Eğer GPIO cihazı hazır değilse veya pin konfigürasyonu sırasında bir hata oluşursa, fonksiyon uygun hata kodunu döndürür.
 * 
 * @param GPIOx       GPIO pin yapılandırma bilgilerini içeren yapı. Bu yapıda port ve pin numarası bulunur.
 * @param extra_flags Konfigurasyon sırasında kullanılacak ekstra bayraklar. Örneğin, input/output ayarları gibi.
 * 
 * @return - `GPIO_SUCCESS`               Eğer pin yapılandırma başarılı bir şekilde yapılırsa.
 * @return - `GPIO_NULL_PTR`            Eğer verilen GPIOx parametresi veya port geçersiz (NULL) ise.
 * @return - `GPIO_NOT_READY`            Eğer belirtilen GPIO cihazı hazır değilse.
 * @return - `GPIO_PIN_CONFIG_FAILED`    Eğer pin yapılandırması sırasında hata oluşursa.
 *
 * @note Bu fonksiyon sadece temel GPIO pin konfigürasyonu yapar. Diğer ileri düzey pin ayarları (interrupt vs.) 
 *       için ek fonksiyonlar kullanılmalıdır.
 */
gpio_status_t gpio_configure_basic(const struct gpio_dt_spec* GPIOx , const gpio_flags_t extra_flags)
{
    if(GPIOx == NULL || GPIOx->port == NULL)
    {
        LOG_ERR("[%s] HATA: GPIO pointer'i veya port'u NULL!", __func__);
        return GPIO_NULL_PTR;
    }
    LOG_INF("[%s] GPIO konfigurasyonu baslatildi. Port: %s, Pin: %d", __func__, GPIOx->port->name, GPIOx->pin);
    
    if(!device_is_ready(GPIOx->port))
    {
        LOG_ERR("[%s] HATA: GPIO cihazinin hazir olmadigi tespit edildi! Port: %s, Pin: %d. [Dosya: %s, Satir: %d]", 
                        __func__, GPIOx->port->name, GPIOx->pin, __FILE__, __LINE__);
        return GPIO_NOT_READY;                
    }
    LOG_INF("[%s] GPIO Port: %s hazir durumda, konfigurasyon baslatiliyor...", __func__, GPIOx->port->name);
    int status = gpio_pin_configure_dt(GPIOx,extra_flags);
    if(status)
    {
        LOG_ERR("[%s] HATA: GPIO pini ayarlanamadi! Hata Kodu: %d. Port: %s, Pin: %d, Ekstra Bayraklar: 0x%x. [Dosya: %s, Satir: %d]",
                        __func__, status, GPIOx->port->name, GPIOx->pin, extra_flags, __FILE__, __LINE__);
        return GPIO_PIN_CONFIG_FAILED;
    }
    LOG_INF("[%s] GPIO konfigurasyonu basariyla tamamlandi. Port: %s, Pin: %d, Ekstra Bayraklar: 0x%x", 
                __func__, GPIOx->port->name, GPIOx->pin, extra_flags);

    return GPIO_SUCCESS;

}


/**
 * @brief GPIO pin için interrupt yapılandırması yapar.
 *
 * Bu fonksiyon, belirtilen GPIO pin'ine interrupt yapılandırması yapar. 
 *
 * @param GPIOx             GPIO pin yapılandırma bilgilerini içeren yapı. Bu yapıda port ve pin numarası bulunur.
 * @param trigger_type      Interrupt tetikleme türü. Örneğin, rising edge, falling edge vb. 
 * @param callback_config   GPIO callback yapılandırma bilgilerini içeren yapı. 
 * @param handler_func      GPIO interrupt tetiklediğinde çağrılacak olan callback fonksiyonu.
 * 
 * @return - `GPIO_SUCCESS`               Eğer interrupt yapılandırması başarılı bir şekilde yapılırsa.
 * @return - `GPIO_NULL_PTR`             Eğer callback yapılandırması veya handler fonksiyonu NULL ise.
 * @return - `GPIO_PIN_CONFIG_FAILED`    Eğer pin yapılandırması sırasında hata oluşursa.
 * @return - `GPIO_CONFIG_INTERRUPT_FAILED`  Eğer interrupt yapılandırması sırasında hata oluşursa.
 * @return - `GPIO_CONFIG_CALLBACK_FAILED`   Eğer callback fonksiyonu eklenemezse.
 *
 * 
 *      
 */
gpio_status_t configure_interrupt(  const struct gpio_dt_spec*              GPIOx , 
                                    gpio_flags_t                            trigger_type , 
                                    struct gpio_callback*                   callback_config ,
                                    gpio_callback_handler                   handler_func)
                                    
{
    if(callback_config == NULL)
    {
        LOG_ERR("[%s] HATA: Callback yapisi NULL!", __func__);
        return GPIO_NULL_PTR;
    }
    if(handler_func == NULL)
    {
        LOG_ERR("[%s] HATA: Callback fonksiyonu NULL!", __func__);
        return GPIO_NULL_PTR;
    }
    gpio_status_t status = gpio_configure_basic(GPIOx, GPIO_INPUT);
    if(status != GPIO_SUCCESS)
    {
        LOG_ERR("[%s] HATA: GPIO konfigurasyonu basarisiz oldu! Hata Kodu: %d. Port: %s, Pin: %d. [Dosya: %s, Satir: %d]",
                        __func__, status, GPIOx->port->name, GPIOx->pin, __FILE__, __LINE__);
        return status;
    }
    int err = gpio_pin_interrupt_configure_dt(GPIOx ,trigger_type ) ; 
    if(err != 0)
    {
        LOG_ERR("[%s] HATA: GPIO interrupt ayarlari basarisiz oldu! Hata Kodu: %d. Port: %s, Pin: %d. [Dosya: %s, Satir: %d]",
                        __func__, err, GPIOx->port->name, GPIOx->pin, __FILE__, __LINE__);
        return GPIO_CONFIG_INTERRUPT_FAILED;
    }
    gpio_init_callback(callback_config, handler_func, BIT(GPIOx->pin));
    err = gpio_add_callback(GPIOx->port, callback_config);
    if(err != 0)
    {
        LOG_ERR("[%s] HATA: Callback eklenemedi! Hata Kodu: %d. Port: %s, Pin: %d. [Dosya: %s, Satir: %d]",
                        __func__, err, GPIOx->port->name, GPIOx->pin, __FILE__, __LINE__);
        return GPIO_CONFIG_CALLBACK_FAILED;
    }

    return GPIO_SUCCESS;

}                                            