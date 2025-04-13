#include <zephyr/kernel.h>
#include<zephyr/logging/log.h>
#include<zephyr/init.h>
#include"gpio_driver.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/*
        Workqueue stack size ve priority tanımlanıyor
*/
#define WORKQUEUE_STACK_SIZE 1024       
#define WORKQUEUE_PRIORITY 8
/*
        Workqueue için stack belleği tanımlanıyor
*/
K_THREAD_STACK_DEFINE(wq_stack, WORKQUEUE_STACK_SIZE);
struct k_work_q wq;

#define INIT_SYSTEM_PRIORITY  41

void workH_led_blink(struct k_work* work);
void timer_handler(struct k_timer *dummy);

K_TIMER_DEFINE(led_timer, timer_handler, NULL);  // Timer tanımlanıyor
K_WORK_DEFINE(workQ_led_blink,workH_led_blink);  // Workqueue ye eklenecek work tanımlanıyor. 


const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);


int init_system(struct device* dev)
{
        /*
                Workqueue başlatılıyor
        */
        k_work_queue_init(&wq);                 
        k_work_queue_start(&wq, wq_stack, WORKQUEUE_STACK_SIZE, WORKQUEUE_PRIORITY, NULL);
        gpio_configure_basic(&led, GPIO_OUTPUT_ACTIVE);
        k_timer_start(&led_timer, K_MSEC(1000), K_MSEC(1000)); // Timer başlatılıyor
        return 0;
}
SYS_INIT(init_system, APPLICATION, INIT_SYSTEM_PRIORITY);


void workH_led_blink(struct k_work* work)
{

        LOG_DBG("LED Blink");
        gpio_pin_toggle_dt(&led);
    
}
void timer_handler(struct k_timer *dummy)
{
        LOG_DBG("Timer Handler");
        /*
                Workqueue ye workQ_led_blink ekleniyor
                Workqueue ye eklenen work, workH_led_blink fonksiyonunu çağıracak    
        */
        if(k_work_submit_to_queue(&wq, &workQ_led_blink) < 0)
        {
                LOG_ERR("workQ_led_blink submit failed");
                return;
        } 
        LOG_DBG("workQ_led_blink submitted");
}




