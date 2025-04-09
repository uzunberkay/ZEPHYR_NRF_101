#include <zephyr/kernel.h>    
#include <zephyr/drivers/gpio.h>      
#include <zephyr/logging/log.h>      

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF); 

/*
    Device Tree'de tanımlı olan 'sw0' ve 'led0' alias'ları üzerinden,
    buton ve LED'e ait GPIO yapılarını alıyoruz.
    Bu sayede hangi pine bağlı oldukları, hangi GPIO portuna ait oldukları gibi bilgiler otomatik gelir.
*/
const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);


/*
    Bu fonksiyon, butona basıldığında tetiklenir.
    gpio_callback yapısı sayesinde bu fonksiyon kesme (interrupt) olarak çağrılır.
*/
void button_pressed(const struct device* port, struct gpio_callback* cb, gpio_port_pins_t pins)
{
    LOG_INF("Butona basildi.");
    gpio_pin_toggle_dt(&led); // LED durumunu değiştir (yanıyorsa söndür, sönükse yak)
}


int main(void)
{
    // LED bağlı olduğu GPIO portu hazır mı kontrol edilir
    if (!gpio_is_ready_dt(&led))
    {
        LOG_ERR("Hata: %s cihazi hazir degil.", led.port->name);
        return -1;
    }

    // LED pinini çıkış olarak yapılandırıyoruz ve başlangıçta aktif hale getiriyoruz (HIGH)
    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        LOG_ERR("Hata: %s pinini yapilandirirken hata oluştu.", led.port->name);
        return ret;
    }

    // gpio_callback yapısı için static tanımlama (heap/stack problemi yaşamamak için)
    static struct gpio_callback button_cb;

    // Buton bağlı olduğu GPIO portu hazır mı kontrol edilir
    if (!gpio_is_ready_dt(&button))
    {
        LOG_ERR("Hata: %s cihazi hazir degil.", button.port->name);
        return -1;
    }

    // Buton pinini giriş (input) olarak yapılandırıyoruz
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0)
    {
        LOG_ERR("Hata: %s pinini yapilandirirken hata oluştu.", button.port->name);
        return ret;
    }

    // Buton için kesme yapılandırması yapılır: Yükselen kenar (butona basıldığında) tetiklenir
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_RISING);
    if (ret < 0)
    {
        LOG_ERR("Hata: %s pinini kesme yapilandirirken hata oluştu.", button.port->name);
        return ret;
    }

    // gpio_callback yapısını başlatıyoruz: Hangi pin ve hangi fonksiyonla eşleştiğini belirtiyoruz
    gpio_init_callback(&button_cb, button_pressed, BIT(button.pin));

    // Oluşturduğumuz callback yapısını, butonun portuna bağlıyoruz
    ret = gpio_add_callback(button.port, &button_cb);
    if (ret != 0)
    {
        LOG_ERR("Hata: %s kesme eklenirken hata oluştu.", button.port->name);
        return ret;
    }
    
    while (true)
    {
        k_msleep(1000);
    }

    return 0;
}
