#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include<zephyr/drivers/gpio.h>



typedef enum {
    GPIO_SUCCESS                    =    0,
    GPIO_NULL_PTR                   =    1,          
    GPIO_NOT_READY                  =    2,           
    GPIO_PIN_CONFIG_FAILED          =    3,           
    GPIO_ERROR                      =    4,           
    GPIO_CONFIG_INTERRUPT_FAILED    =    5,           
    GPIO_CONFIG_CALLBACK_FAILED     =    6,      
    GPIO_HANDLER_FUNC_NULL          =    7                    
} gpio_status_t;

/**
 * @brief Callback fonksiyon tipi
 *
 * Bu fonksiyon tipi, bir GPIO interrupt tetiklendiğinde çağrılır.
 */
typedef void(*gpio_callback_handler)(const struct device*,struct gpio_callback*,uint32_t);



gpio_status_t gpio_configure_basic(const struct gpio_dt_spec* GPIOx , const gpio_flags_t extra_flags);
gpio_status_t configure_interrupt(  const struct gpio_dt_spec*              GPIOx , 
    gpio_flags_t                            trigger_type , 
    struct gpio_callback*                   callback_config ,
    gpio_callback_handler                   handler_func);



#endif // GPIO_DRIVER_H