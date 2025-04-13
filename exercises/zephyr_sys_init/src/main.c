#include <zephyr/kernel.h>
#include<zephyr/init.h>
#include"gpio_driver.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

#define SYS_INIT_PRIORITY_DEFAULT 41

#define THREAD_PRIORITY 5
#define STACK_SIZE 1024
#define THREAD_SLEEP_TIME 1000


const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);


int gpio_init(const struct device* dev)
{
        LOG_DBG("Gpio init işlemlerini burada yapiyoruz");
        return gpio_configure_basic(&led,GPIO_OUTPUT_ACTIVE);
}

SYS_INIT(gpio_init, APPLICATION, SYS_INIT_PRIORITY_DEFAULT);


void thread_function(void* vp1,void* vp2 ,void* vp3)
{
        ARG_UNUSED(vp1);
        ARG_UNUSED(vp2);
        ARG_UNUSED(vp3);
        while(1)
        {
                gpio_pin_toggle_dt(&led);
                k_msleep(THREAD_SLEEP_TIME);
        }
}

K_THREAD_DEFINE(thread_id, STACK_SIZE, thread_function, NULL, NULL, NULL,
        THREAD_PRIORITY, 0, 0);