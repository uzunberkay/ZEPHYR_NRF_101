#include <zephyr/kernel.h>
#include"gpio_driver.h"
#include<zephyr/logging/log.h>

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

const struct gpio_dt_spec thread1_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec thread2_led = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

/*
        Threadler için ayrılan yığın boyutları ve öncelikleri tanımlanır.
*/
#define THREAD1_STACK_SIZE 1024
#define THREAD2_STACK_SIZE 1024

#define THREAD1_PRIORITY 7
#define THREAD2_PRIORITY 8



void thread1_func(void* vp1,void* vp2,void* vp3)
{
        ARG_UNUSED(vp1);
        ARG_UNUSED(vp2);
        ARG_UNUSED(vp3);
        if(gpio_configure_basic(&thread1_led,GPIO_OUTPUT_ACTIVE) != GPIO_SUCCESS)
        {
                LOG_ERR("thread1_led konfigurasyonu basarisiz!");
                return;
        }
        while(1)
        {
                gpio_pin_toggle_dt(&thread1_led);
                LOG_DBG("Thread 1 LED toggle");
                k_msleep(500);
        }
}
/*
        Thread 1'i tanımla  K_THREAD_DEFINE makrosu ile
        Bu makro, thread1_id adında bir thread tanımlar ve thread1_func fonksiyonunu çalıştırır.
        Thread 1 için gerekli parametreleri ve önceliği ayarlar.
        Thread 1 için gerekli yığın boyutunu ve önceliğini ayarlar.
*/
K_THREAD_DEFINE(thread1_id, THREAD1_STACK_SIZE, thread1_func, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);



void thread2_func(void* vp1,void* vp2,void* vp3)
{
        ARG_UNUSED(vp1);
        ARG_UNUSED(vp2);
        ARG_UNUSED(vp3);
        if(gpio_configure_basic(&thread2_led,GPIO_OUTPUT_ACTIVE) != GPIO_SUCCESS)
        {
                LOG_ERR("thread2_led konfigurasyonu basarisiz!");
                return;
        }
        while(1)
        {
                gpio_pin_toggle_dt(&thread2_led);
                LOG_DBG("Thread 2 LED toggle");
                k_msleep(500);
        }
}

/*
        Thread 2'i tanımla  K_THREAD_DEFINE makrosu ile
        Bu makro, thread1_id adında bir thread tanımlar ve thread1_func fonksiyonunu çalıştırır.
        Thread 2 için gerekli parametreleri ve önceliği ayarlar.
        Thread 2 için gerekli yığın boyutunu ve önceliğini ayarlar.
*/
K_THREAD_DEFINE(thread2_id, THREAD2_STACK_SIZE, thread2_func, NULL, NULL, NULL, THREAD2_PRIORITY, 0, 0);




