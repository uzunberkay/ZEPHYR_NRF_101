 #include <zephyr/kernel.h>
 #include <zephyr/device.h>
 #include <zephyr/devicetree.h>
 #include <zephyr/logging/log.h>
 #include <zephyr/drivers/adc.h>
 

 static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));
 
 LOG_MODULE_REGISTER(adc, LOG_LEVEL_DBG);

int readAdcValue(struct adc_dt_spec* adc_channel,struct adc_sequence* sequence);
 
int main(void)
{
        int16_t val;
        struct adc_sequence sequence = {
		.buffer = &val,
		.buffer_size = sizeof(val),
	};
        if (!device_is_ready(adc_channel.dev)) {
                LOG_ERR("ADC cihazi (%s) kullanima hazir değil", adc_channel.dev->name);
                return -1;
        }
        int ret = adc_channel_setup_dt(&adc_channel);
        if(ret < 0)
        {
                LOG_ERR("ADC yapilandirma hatasi (%s): %d", adc_channel.dev->name, ret);
                return ret;
        }
        ret = adc_sequence_init_dt(&adc_channel, &sequence);
        if (ret < 0) {
                LOG_ERR("ADC cihazi (%s) ayarlama hatasi: %d", adc_channel.dev->name, ret);
                return ret;
        }

        while(true)
        {

                LOG_DBG("ADC cihazi (%s) deger: %dmV", adc_channel.dev->name, readAdcValue(&adc_channel,&sequence));
                k_msleep(1000);
        }

        return 0;
}



/**
 * @brief ADC kanalından bir değer okur ve miliVolt (mV) cinsine çevirir.
 *
 * Bu fonksiyon verilen ADC kanalından veri okur, ham değeri alır
 * ve bunu miliVolt cinsine çevirir. Hatalı durumda -1 döner.
 *
 * @param adc_channel ADC kanalını tanımlayan yapı (adc_dt_spec).
 * @param sequence ADC okuma ayarlarını içeren yapı (adc_sequence).
 *
 * @return - `mV` cinsinden ölçülen değer
 * @return - `-1`: Okuma veya dönüştürme hatası
 * 
 * 
 * 
 */
 int readAdcValue(struct adc_dt_spec* adc_channel,struct adc_sequence* sequence)
{
        if(adc_read(adc_channel->dev, sequence) < 0)
        {
                LOG_ERR("ADC cihazi (%s) okuma hatasi", adc_channel->dev->name);
                return -1;
        }
        int16_t raw = *(int16_t*)sequence->buffer;
        int mV = raw;
        if(adc_raw_to_millivolts_dt(adc_channel, &mV) < 0)
        {
                LOG_ERR("ADC cihazi (%s) mV donusturme hatasi", adc_channel->dev->name);
                return -1;
        }

        return mV;
        
}