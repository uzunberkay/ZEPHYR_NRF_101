#include <zephyr/kernel.h>
#include"gpio_driver.h"
#include<zephyr/logging/log.h>

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);


const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);


void button_handler(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
        LOG_DBG("Butona basildi.");
        gpio_pin_toggle_dt(&led);
}


int main(void)
{
        struct gpio_callback button_callback;
        gpio_configure_basic(&led, GPIO_OUTPUT_ACTIVE);
        configure_interrupt(&button, GPIO_INT_EDGE_TO_ACTIVE, &button_callback, button_handler);
        while(true)
        {
                k_msleep(1000);
        }
        return 0;
}
