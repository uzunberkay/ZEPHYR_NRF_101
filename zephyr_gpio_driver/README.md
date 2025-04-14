
---

# ⚡ Zephyr GPIO Driver

Bu driver, Zephyr RTOS üzerinde GPIO pinlerinin temel yapılandırma ve interrupt işlevselliğini sağlamak amacıyla geliştirilmiştir. Kullanıcıların, cihazlarının GPIO pinlerini giriş/çıkış olarak yapılandırmasına, ayrıca GPIO interrupt işlevselliklerini etkinleştirmesine olanak tanır.

### Yapılan İşlemler:

- **GPIO Pin Yapılandırması**: GPIO pinlerinin temel yapılandırmalarını gerçekleştirir. Bu işlem, belirli bir GPIO pininin giriş ya da çıkış olarak yapılandırılması için kullanılır. 
  
- **GPIO Interrupt Yapılandırması**: Belirli bir GPIO pinine interrupt özelliği ekler. Tetikleme türleri (örneğin, rising edge, falling edge) belirlenebilir ve interrupt tetiklendiğinde çağrılacak callback fonksiyonları yapılandırılabilir.

### Kullanılabilir Fonksiyonlar:

#### 1. GPIO Pin Yapılandırması
`gpio_configure_basic()` fonksiyonu, bir GPIO pinini belirli bayraklarla (input/output) yapılandırmak için kullanılır.

```c
gpio_status_t gpio_configure_basic(const struct gpio_dt_spec* GPIOx, const gpio_flags_t extra_flags);
```

Bu fonksiyon, pin yapılandırmasında yaşanabilecek hata durumlarına karşı uygun hata kodlarını döndürür. Eğer GPIO cihazı hazır değilse veya pin konfigürasyonu başarısız olursa, fonksiyon hata kodu döndürecektir.

#### 2. GPIO Interrupt Yapılandırması
`configure_interrupt()` fonksiyonu, bir GPIO pinine interrupt işlevselliği ekler. Bu fonksiyon, belirli bir tetikleme türü ve callback fonksiyonu ile yapılandırılır.

```c
gpio_status_t configure_interrupt(const struct gpio_dt_spec* GPIOx, gpio_flags_t trigger_type, struct gpio_callback* callback_config, gpio_callback_handler handler_func);
```

Bu fonksiyon, GPIO pinine interrupt özelliği ekledikten sonra, interrupt tetiklendiğinde çağrılacak callback fonksiyonunu belirler ve ilgili callback yapılandırmasını GPIO portuna ekler.

### Hata Kodları:

- `GPIO_SUCCESS`: İşlem başarıyla tamamlandı.
- `GPIO_NULL_PTR`: Geçersiz bir parametre (NULL değeri) sağlandı.
- `GPIO_NOT_READY`: Belirtilen GPIO cihazı hazır değil.
- `GPIO_PIN_CONFIG_FAILED`: Pin yapılandırması başarısız oldu.
- `GPIO_CONFIG_INTERRUPT_FAILED`: Interrupt yapılandırması başarısız oldu.
- `GPIO_CONFIG_CALLBACK_FAILED`: Callback fonksiyonu eklenemedi.

