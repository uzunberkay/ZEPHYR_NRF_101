---

## ⚡Zephyr LED Blink 

Bu örnekte, **Zephyr RTOS** ve **nRF52840 kartı** kullanarak bir LED'i belirli aralıklarla yakıp söndüreceğiz. Örnek uygulamamızda **Device Tree**, **GPIO sürücüleri**, **konfigürasyon dosyaları** ve **loglama sistemi** gibi Zephyr bileşenleri kullanılacaktır.

---

## 🧾 1. `prj.conf` (KConfig) Dosyası ve Anlamı

`prj.conf` dosyası, Zephyr uygulamamızın yapılandırma ayarlarını içerir. Bu dosyada yapılan her değişiklik, derleme sırasında uygulamamızın özelliklerini aktive eder veya devre dışı bırakır. Bizim uygulamamızda temel olarak şu satırlara yer veriyoruz:

```plaintext
CONFIG_GPIO=y
CONFIG_LOG=y
```

### Açıklamaları:

- `CONFIG_GPIO=y`: GPIO (General Purpose Input/Output) desteğini aktif eder. LED yakmak için bu özellik gereklidir.
- `CONFIG_LOG=y`: Zephyr'in loglama altyapısını aktif eder.

---

## 📦 2. Device Tree ile GPIO Tanımı

Zephyr, donanım bilgilerini doğrudan C koduna gömmez. Bunun yerine **Device Tree (DT)** yapısını kullanır. Böylece, farklı donanımlar için aynı kaynak kodu çalışabilir.

```c
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
```

### Ne işe yarar?

- `DT_ALIAS(led0)`: Device Tree'deki `led0` alias'ını bulur (bu tanım genellikle board dosyasındaki yerel yapılandırmada yer alır).
- `GPIO_DT_SPEC_GET(..., gpios)`: Bu alias'a ait GPIO pin ve port bilgilerini getirir.

📌 Bu yapı sayesinde `led` değişkeni, LED'in bağlı olduğu GPIO portu ve pin bilgilerini içerir.

---

## 🧠 3. GPIO Cihazının Hazır Olduğunu Kontrol Etmek

```c
if (!device_is_ready(led.port)) {
    LOG_ERR("Hata: %s cihazı hazır değil.", led.port->name);
    return 0;
}
```

> Burada, LED'in bağlı olduğu GPIO cihazının gerçekten sistem tarafından tanındığı ve hazır olup olmadığını kontrol ediyoruz. Eğer donanım uygun değilse, programın çalışması durduruluyor.

---

## ⚙️ 4. GPIO Pinini Konfigüre Etmek

```c
gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
```

- Bu satır, `led` pinini çıkış (output) olarak yapılandırır.
- `GPIO_OUTPUT_ACTIVE`: Başlangıçta pinin aktif (HIGH) olmasını sağlar.

---

## 🔁 5. LED’i Yakıp Söndürmek

```c
while (1) {
    gpio_pin_toggle_dt(&led);
    k_msleep(1000);
}
```

- `gpio_pin_toggle_dt(&led)`: Pinin seviyesini değiştirir. Eğer pin aktifse (HIGH) pasif hale (LOW) gelir, pasifse (LOW) aktif olur (HIGH).
- `k_msleep(1000)`: 1000 milisaniye (1 saniye) bekler.

Bu döngü sayesinde LED 1 saniyede bir yanıp söner.

---

