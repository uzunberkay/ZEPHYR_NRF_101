
---

## ⚡Zephyr ADC 

Bu projede **Zephyr RTOS** üzerinde çalışan bir **nRF52840** geliştirme kartı kullanılarak analog sinyallerin dijital verilere dönüştürülmesi gösterilmektedir. ADC modülü, belirli bir analog pini örnekleyerek milivolt cinsinden veri sağlar. Bu sayede sensörlerden ya da dış dünyadan gelen analog sinyaller yazılım tarafından işlenebilir hale gelir.

---

## 🧾 1. `prj.conf`(KConfig) Dosyası

Proje yapılandırma dosyasında ADC ve loglama altyapısı aktif edilmiştir:

```plaintext
CONFIG_ADC=y
CONFIG_LOG=y
```

### Açıklamaları:

- `CONFIG_ADC=y`: ADC sürücülerini etkinleştirir.
- `CONFIG_LOG=y`: Zephyr loglama altyapısını aktif eder. ADC değerleri log üzerinden takip edilebilir.

---

## 🗺️ 2.  DeviceTree (Overlay) Dosyası

Donanımın ADC yapılandırması `.overlay` dosyası üzerinden gerçekleştirilmiştir. Bu sayede ADC kanalı ve giriş pini device tree yapısı içinde tanımlanır.

```dts
/ {
	zephyr,user {
		io-channels = <&adc 0>;
	};
};

&adc {
	#address-cells = <1>;
	#size-cells = <0>;
	status = "okay";
	channel@0 {
		reg = <0>;
		zephyr,gain = "ADC_GAIN_1_6";
		zephyr,reference = "ADC_REF_INTERNAL";
		zephyr,acquisition-time = <ADC_ACQ_TIME_DEFAULT>;
		zephyr,input-positive = <NRF_SAADC_AIN0>; 
		zephyr,resolution = <12>;
	};
};
```
