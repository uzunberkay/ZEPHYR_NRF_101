Anladım! O zaman, örnek README'yi takip ederek butonla LED kontrolü yapan projeniz için benzer bir formatta bir README dosyası oluşturacağım. İşte önerilen README:

---

## 🔘 Button Controlled LED 

Bu örnekte, **Zephyr RTOS** ve **nRF52840 kartı** kullanarak bir **buton** ile kontrol edilen **LED**'i yapılandıracağız. Butona her basıldığında, LED'in durumu değişir (yanıyorsa söner, sönükse yanar). Bu uygulama, **GPIO**, **Interrupts (Kesme)**, **Device Tree**, ve **Logging** gibi Zephyr bileşenlerini kullanır.

---

## 🧾 1. `prj.conf` Dosyası ve Anlamı

`prj.conf` dosyası, Zephyr uygulamamızın yapılandırma ayarlarını içerir. Bu dosyada yapılan her değişiklik, uygulamanın çeşitli bileşenlerini aktive eder veya devre dışı bırakır. Bu uygulama için kullandığımız temel ayarlar şunlardır:

```plaintext
CONFIG_GPIO=y
CONFIG_LOG=y
```

### Açıklamaları:

- `CONFIG_GPIO=y`: GPIO (General Purpose Input/Output) desteğini aktif eder. LED ve buton için gerekli GPIO işlevselliğini sağlar.
- `CONFIG_LOG=y`: Zephyr'in loglama altyapısını aktif eder. Hata ve bilgi mesajlarının yazılmasını sağlar.

---

## 📦 2. Device Tree ile GPIO Tanımlaması

Zephyr, donanım bilgilerini doğrudan C koduna gömmek yerine **Device Tree (DT)** yapısını kullanır. Bu, kodun farklı donanımlarla uyumlu olmasını sağlar.

```c
const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
```

### Ne işe yarar?

- `DT_ALIAS(sw0)`: Device Tree'deki `sw0` alias'ını bulur (bu, genellikle board dosyasındaki yerel yapılandırmada yer alır).
- `GPIO_DT_SPEC_GET(..., gpios)`: Bu alias'a ait GPIO pin ve port bilgilerini getirir.

📌 Bu yapı sayesinde, `button` ve `led` değişkenleri, buton ve LED'in bağlı olduğu GPIO portları ve pin bilgilerini içerir.

---

## 🧠 3. GPIO Cihazının Hazır Olduğunu Kontrol Etmek

Buton ve LED cihazlarının sistem tarafından tanınıp tanınmadığını kontrol ederiz:

```c
if (!gpio_is_ready_dt(&led)) {
    LOG_ERR("Hata: %s cihazı hazır değil.", led.port->name);
    return -1;
}

if (!gpio_is_ready_dt(&button)) {
    LOG_ERR("Hata: %s cihazı hazır değil.", button.port->name);
    return -1;
}
```

> Burada, LED ve butonun bağlı olduğu GPIO cihazlarının gerçekten hazır olup olmadığını kontrol ediyoruz. Eğer herhangi biri hazır değilse, program çalışmayı durdurur.

---

## ⚙️ 4. GPIO Pinlerini Konfigüre Etmek

Buton ve LED pinlerini uygun şekilde yapılandırıyoruz:

```c
gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
gpio_pin_configure_dt(&button, GPIO_INPUT);
```

- `GPIO_OUTPUT_ACTIVE`: LED pinini çıkış olarak yapılandırır ve başlangıçta aktif (HIGH) yapar.
- `GPIO_INPUT`: Buton pinini giriş olarak yapılandırır.

---

## 🔔 5. Buton Basıldığında LED Durumunu Değiştirmek

Butona her basıldığında, **LED'in durumu değişecektir** (yanıyorsa söner, sönükse yanar). Bunun için bir kesme (interrupt) fonksiyonu kullanıyoruz:

```c
void button_pressed(const struct device* port, struct gpio_callback* cb, gpio_port_pins_t pins) {
    LOG_INF("Butona basıldı.");
    gpio_pin_toggle_dt(&led); // LED durumunu değiştir (yanıyorsa söndür, sönükse yak)
}
```

Bu fonksiyon, butona basıldığında tetiklenir ve LED'in durumunu değiştirir.

---

## 🧩 6. GPIO Kesme Yapılandırması

Buton pinini **kesme** (interrupt) olarak yapılandırıyoruz. Yükselen kenar (butona basıldığında) tetiklenir:

```c
ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_RISING);
if (ret < 0) {
    LOG_ERR("Hata: %s pinini kesme yapılandırırken hata oluştu.", button.port->name);
    return ret;
}

gpio_init_callback(&button_cb, button_pressed, BIT(button.pin));
ret = gpio_add_callback(button.port, &button_cb);
if (ret != 0) {
    LOG_ERR("Hata: %s kesme eklenirken hata oluştu.", button.port->name);
    return ret;
}
```

- `GPIO_INT_EDGE_RISING`: Butona basıldığında (yükselen kenar) kesme tetiklenir.

---

