#include <zephyr/kernel.h>
#include"gpio_driver.h"
#include<zephyr/logging/log.h>  
#include<zephyr/init.h>

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

#define SYSTEM_INIT_PRIO        43
#define SAMPLE_THREAD_STACK_SIZE        1024
#define SAMPLE_THREAD_PRIO              7

struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);


/*
        Semaforu tanımlıyoruz.
*/
K_SEM_DEFINE(sample_sem,0,1); 

void timer_cb(struct k_timer* dummy);
void sample_thread(void* vp1,void *vp2, void* vp3);


int system_init(struct device* dev)
{
        ARG_UNUSED(dev);
        gpio_configure_basic(&led,GPIO_OUTPUT_ACTIVE);
   
        return 0;
}


SYS_INIT(system_init,APPLICATION,SYSTEM_INIT_PRIO);
K_TIMER_DEFINE(sample_timer,timer_cb,NULL);
K_THREAD_DEFINE(sample_thread_id,SAMPLE_THREAD_STACK_SIZE,sample_thread,NULL,NULL,NULL,
        SAMPLE_THREAD_PRIO,0,0);

void timer_cb(struct k_timer* dummy)
{
        LOG_DBG("Timer callback");
        k_sem_give(&sample_sem);
}


void sample_thread(void* vp1,void *vp2, void* vp3)
{
        ARG_UNUSED(vp1);
        ARG_UNUSED(vp2);
        ARG_UNUSED(vp3);

        while(true)
        {
                /*
                        THREAD 
                */
                k_sem_take(&sample_sem,K_FOREVER);
                gpio_pin_toggle_dt(&led);
        }
}