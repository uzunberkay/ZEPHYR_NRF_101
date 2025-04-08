#include <zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>
#include<zephyr/logging/log.h>

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

/*
    'led0' alias'ına ait GPIO pin özelliklerini almak için Device Tree kullanılır.
*/
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

#define SLEEP_TIME_MS 1000

int main(void)
{
        /*
                LED'in bağlı olduğu GPIO cihazının hazır olup olmadığını kontrol eder.
        */
        if(!device_is_ready(led.port))
        {
            LOG_ERR("Hata: %s cihazi hazir degil.", led.port->name);
            return 0;
        }
        /*
                Belirtilen GPIO pinini yapılandırır. GPIO_OUTPUT_ACTIVE, pinin başlangıçta aktif olmasını sağlar.
                GPIO_OUTPUT, pinin çıkış olarak yapılandırılmasını sağlar.
        */
        int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
        if(ret < 0)
        {
            LOG_ERR("Hata %d: pin %d yapilandirilamadi", ret, led.pin);
            return 0;
        }
        LOG_INF("Pin %d yapilandirildi", led.pin);
        
        while(1)
        {
        /*
                gpio_pin_toggle_dt, GPIO pininin durumunu değiştirir.
                Eğer pin aktifse kapatır, kapalıysa açar.
        */ 
                gpio_pin_toggle_dt(&led);
        /*
                k_msleep, belirtilen süre boyunca mevcut thread'i beklemeye alır.
        */
                k_msleep(SLEEP_TIME_MS);
        }

        
}
