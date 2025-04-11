#include <zephyr/kernel.h>
#include"gpio_driver.h"
#include<zephyr/init.h>


#define GPIO_INIT_PRIORITY_DEFAULT 41

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);



void timer_handler(struct k_timer *dummy)
{
        
        gpio_pin_toggle_dt(&led);
}
/*
        Timeri tanımlıyoruz:
        K_TIMER_DEFINE (timerIsim,HandlerFunc,TimerStopEttiğindeHandlerFunc);     
*/
K_TIMER_DEFINE(sample_timer, timer_handler, NULL);                              

int gpio_init(const struct device* dev)
{
        ARG_UNUSED(dev);
        /*      Timerı başlatıyoruz:
                K_MSEC(1000): Başlangıç zamanını 1000ms (1 saniye) olarak ayarlıyoruz
                K_MSEC(1000): Zamanlayıcının periyodunu 1000ms (1 saniye) olarak ayarlıyoruz
        */
        k_timer_start(&sample_timer, K_MSEC(1000), K_MSEC(1000));               
        return gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

}

SYS_INIT(gpio_init, APPLICATION, GPIO_INIT_PRIORITY_DEFAULT);




