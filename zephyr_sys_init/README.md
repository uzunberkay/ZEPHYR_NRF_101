
---

## 🔧 SYS_INIT Kullanımı

**Zephyr RTOS**, uygulama başlatılmadan önce belirli fonksiyonların başlatılmasını sağlamak için `SYS_INIT()` makrosunu kullanır. Bu, özellikle donanım başlatma ve sistem konfigürasyon işlemleri için kullanılır. `SYS_INIT()` fonksiyonu, bir sistem bileşeninin belirli bir öncelik sırasına göre başlatılmasını sağlar.

---

## 🧾 1. SYS_INIT Makrosu Nedir?

`SYS_INIT()` makrosu, belirli bir fonksiyonu belirli bir zamanda çağırarak, sistemin başlatılmasında veya konfigürasyonunda yardımcı olur. Bu, özellikle **donanım başlatma**, **giriş/çıkış (I/O)** işlemleri ve **donanım sürücülerinin** başlatılması gibi durumlarda kullanılır. `SYS_INIT()` ile belirlenen fonksiyonlar, **uygulama** başlatılmadan önce çalıştırılır.

---

## ⚙️ 2. SYS_INIT Parametreleri

`SYS_INIT()` makrosu üç ana parametre alır:

```c
SYS_INIT(function_name, module_name, priority);
```

- **`function_name`**: Başlatılacak fonksiyonun adı.
- **`module_name`**: Fonksiyonun ait olduğu sistem modülünün adı.
- **`priority`**: Fonksiyonun ne zaman çalıştırılacağını belirler. Daha düşük sayılar, daha yüksek önceliği ifade eder.

---

##  3. SYS_INIT Kullanımı

Aşağıdaki örnekte, GPIO başlatma fonksiyonu bir SYS_INIT fonksiyonu olarak kullanılmıştır. Bu fonksiyon, uygulama başlamadan önce GPIO yapılandırmasını gerçekleştirecektir.

### Kod Örneği:

```c
int gpio_init(const struct device *dev)
{
    LOG_DBG("GPIO init işlemleri burada yapılacak");
    return gpio_configure_basic(&led, GPIO_OUTPUT_ACTIVE);
}

SYS_INIT(gpio_init, APPLICATION, SYS_INIT_PRIORITY_DEFAULT);



