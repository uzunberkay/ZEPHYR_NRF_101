#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(msg_queue_example, LOG_LEVEL_DBG);


#define MSG_QUEUE_MAX_MSGS         10
#define MSG_QUEUE_ALIGNMENT        1

#define PRODUCER_STACK_SIZE        1024
#define PRODUCER_PRIORITY          7
#define CONSUMER_STACK_SIZE        1024
#define CONSUMER_PRIORITY          8
#define PRODUCER_INTERVAL_MS       1000
#define CONSUMER_INTERVAL_MS       1000
#define QUEUE_TIMEOUT_MS           100


K_MSGQ_DEFINE(sensor_data_queue, sizeof(int), MSG_QUEUE_MAX_MSGS, MSG_QUEUE_ALIGNMENT);


void producer_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (true) {
        int sensor_value  = k_uptime_get_32() % 100;    
        int ret = k_msgq_put(&sensor_data_queue, &sensor_value , K_MSEC(QUEUE_TIMEOUT_MS));

        if (ret == 0) {
            LOG_INF("Sensor verisi kuyruğa eklendi: %d", sensor_value);
        } else {
            LOG_ERR("Mesaj kuyruğa eklenemedi! Hata kodu: %d", ret);
        }

        k_msleep(PRODUCER_INTERVAL_MS);
    }
}


void consumer_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    int sensor_value = 0;

    while (true) {
        if (k_msgq_num_used_get(&sensor_data_queue) > 0) {
            int ret = k_msgq_get(&sensor_data_queue, &sensor_value, K_MSEC(QUEUE_TIMEOUT_MS));

            if (ret == 0) {
                LOG_INF("Kuyruktan veri alindi: %d", sensor_value);
            } else {
                LOG_ERR("Kuyruktan veri alinamadi! Hata kodu: %d", ret);
            }
        } else {
            LOG_WRN("Mesaj kuyruğu boş.");
        }

        k_msleep(CONSUMER_INTERVAL_MS);
    }
}


K_THREAD_DEFINE(producer_tid, PRODUCER_STACK_SIZE, producer_thread,
                NULL, NULL, NULL, PRODUCER_PRIORITY, 0, 0);

K_THREAD_DEFINE(consumer_tid, CONSUMER_STACK_SIZE, consumer_thread,
                NULL, NULL, NULL, CONSUMER_PRIORITY, 0, 0);
