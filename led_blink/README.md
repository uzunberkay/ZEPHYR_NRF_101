
## 🔦 LED Blink Uygulaması (Zephyr RTOS)

Bu proje, **Zephyr RTOS** üzerinde GPIO kullanarak bir LED'in belirli aralıklarla yanıp sönmesini sağlar. `led0` alias'ı üzerinden, hedef donanımda tanımlı bir LED'e erişilir ve bu LED her 1 saniyede bir yanıp söner.

---

### 📁 Proje Dosya Yapısı

```
ZEPHYR_NRF_101/
└── led_blink/
    ├── src/
    │   └── main.c
    ├── CMakeLists.txt
    ├── prj.conf
    └── README.md  ← (bu dosya)
```

---

### 🧠 Kullanılan Başlıca Bileşenler

- `gpio_dt_spec`: Device Tree üzerinden GPIO tanımı almak için kullanılır.
- `gpio_pin_configure_dt()`: GPIO pinini çıkış olarak yapılandırır.
- `gpio_pin_toggle_dt()`: Pinin seviyesini değiştirir (toggle).
- `k_msleep()`: Belirtilen milisaniye kadar thread’i bekletir.
- `LOG_MODULE_REGISTER`: Log mekanizmasını aktif eder.

---

### ⚙️ Derleme ve Yükleme

```bash
west build -b <board_name> .
west flash
```

📝 Örnek:

```bash
west build -b nrf52840dk_nrf52840 .
```

---

### 🧾 Kod Açıklaması

```c
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
```
> `led0` alias'ına tanımlı GPIO pinini device tree'den alır.

```c
if (!device_is_ready(led.port))
```
> GPIO aygıtı sistem tarafından hazır mı diye kontrol eder.

```c
gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
```
> GPIO pinini çıkış olarak yapılandırır ve başlangıçta aktif (HIGH) yapar.

```c
while (1) {
    gpio_pin_toggle_dt(&led);
    k_msleep(SLEEP_TIME_MS);
}
```
> LED her saniyede bir yanar ve söner. Sonsuz döngüde bu işlemi tekrar eder.

---

### 📝 `prj.conf` İçeriği Örneği

```ini
CONFIG_GPIO=y
CONFIG_LOG=y
CONFIG_USE_SEGGER_RTT=y
CONFIG_LOG_BACKEND_RTT=y
```

> RTT üzerinden logları görmek için bu ayarları kullanabilirsiniz.

---

### 🧪 Test Edilen Kartlar

- ✅ nRF52840 DK

