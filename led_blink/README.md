## 🔦 LED Blink - Zephyr RTOS ile Temel GPIO Kullanımı

Bu örnekte, **Zephyr RTOS** kullanarak bir LED'i belirli aralıklarla yakıp söndüreceğiz. Örnek uygulamamızda **Device Tree**, **GPIO sürücüleri**, **konfigürasyon dosyaları** ve **loglama sistemi** gibi Zephyr bileşenleri kullanılacaktır.

---

## 🧾 1. `prj.conf` Dosyası ve Anlamı

`prj.conf`, Zephyr uygulamamızın yapılandırma (konfigürasyon) ayarlarını içerir. Buraya yazılan her satır, derleme sırasında belirli özellikleri açar veya kapatır. Bizim uygulamamızda temel olarak şu satırlar yer alır:

CONFIG_GPIO=y
CONFIG_LOG=y

### Açıklamaları:

- `CONFIG_GPIO=y`: GPIO (General Purpose Input/Output) desteğini aktif eder. LED yakmak için buna ihtiyacımız var.
- `CONFIG_LOG=y`: Zephyr'in loglama altyapısını aktif eder.
---

## 📦 2. Device Tree ile GPIO Tanımı

Zephyr, donanım bilgilerini doğrudan C koduna gömmez. Bunun yerine **Device Tree (DT)** yapısını kullanır. Böylece farklı donanımlar için aynı kaynak kodu çalışabilir.

```c
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
```

### Ne işe yarar?

- `DT_ALIAS(led0)`: Device Tree'deki `led0` alias'ını bulur (bu tanım genellikle board dosyasında yer alır).
- `GPIO_DT_SPEC_GET(..., gpios)`: Bu alias’a ait GPIO pin ve port bilgilerini getirir.

📌 Bu yapı sayesinde `led` değişkeni artık hem pin numarasını hem de bağlı olduğu GPIO kontrolcüsünü içerir.

---

## 🧠 3. GPIO Cihazının Hazır Olduğunu Kontrol Etmek

```c
if (!device_is_ready(led.port)) {
    LOG_ERR("Hata: %s cihazi hazir degil.", led.port->name);
    return 0;
}
```

> Burada, LED’in bağlı olduğu GPIO kontrolcüsü gerçekten sistem tarafından tanındı mı, hazır mı diye kontrol ediyoruz. Donanım uygun değilse program erken sonlanır.

---

## ⚙️ 4. GPIO Pinini Konfigüre Etmek

```c
gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
```

- Bu satır, `led` pinini çıkış (output) olarak ayarlıyor.
- `GPIO_OUTPUT_ACTIVE`: Başlangıçta pinin aktif (HIGH) olmasını sağlar.

---

## 🔁 5. LED’i Yakıp Söndürmek

```c
while (1) {
    gpio_pin_toggle_dt(&led);
    k_msleep(1000);
}
```

- `gpio_pin_toggle_dt(&led)`: Pinin seviyesini değiştirir. HIGH ise LOW yapar, LOW ise HIGH.
- `k_msleep(1000)`: 1000 milisaniye (1 saniye) bekler.

Bu döngü sayesinde LED 1 saniyede bir yanıp söner.

---



