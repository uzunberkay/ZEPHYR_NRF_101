
---

# ⚡ Zephyr SYS_INIT 

**`SYS_INIT`** makrosu, Zephyr'de **sistem başlatma** (system initialization) sırasında bir fonksiyonu çağırmak için kullanılır. Bu fonksiyon, belirli bir **öncelik** ile uygulama başlatılmadan önce veya uygulama başlatılırken bir kez çalıştırılmak üzere ayarlanabilir. Bu, özellikle donanım başlatma veya düşük seviyeli konfigürasyon işlemleri için kullanışlıdır.

### SYS_INIT Fonksiyonu Nasıl Çalışır?

- **Makro Kullanımı**: `SYS_INIT(fonksiyon, modül, öncelik)` şeklinde yazılır.
  - **fonksiyon**: Başlatılacak olan fonksiyon.
  - **modül**: Başlatma işleminin ne zaman yapılacağını belirler. `APPLICATION`, uygulama başlatılmadan önce yapılacak işlemler için kullanılır.
  - **öncelik**: Başlatma önceliği. `SYS_INIT_PRIORITY_DEFAULT` gibi bir değer atanabilir.

### Projedeki Uygulama

```c
int gpio_init(const struct device* dev)
{
        ARG_UNUSED(dev);
        LOG_DBG("Gpio init işlemlerini burada yapiyoruz");
        return gpio_configure_basic(&led, GPIO_OUTPUT_ACTIVE);
}

SYS_INIT(gpio_init, APPLICATION, SYS_INIT_PRIORITY_DEFAULT);
```

Yukarıdaki kodda, `gpio_init` fonksiyonu, **sistem başlatıldığında** otomatik olarak çalışacak şekilde ayarlanmıştır. Bu, **`SYS_INIT`** makrosu ile yapılır ve öncelik olarak **`SYS_INIT_PRIORITY_DEFAULT`** verilmiştir. Bu fonksiyon **LED GPIO pinini** yapılandırarak aktif hale getirmeyi amaçlar.

### SYS_INIT’in Avantajları

- **Donanım Başlatma**: Zephyr'de birçok donanım, başlatma işleminde belirli bir sıralama gerektirebilir. `SYS_INIT`, bu sıralamayı kolayca yönetir.
- **Uygulama Başlatılmadan Önce Yapılacak İşlemler**: Sistem başlatılırken kritik konfigürasyonların yapılması gerektiğinde, `SYS_INIT` kullanarak bu işlemler önceden yapılabilir.
- **Bağımlılıklar**: `SYS_INIT` ile başlatılan fonksiyonlar, başka başlatma işlemleri veya donanımlar tarafından bağımlı olabilir.

### Thread ve GPIO Pin Kontrolü

Projede bir **thread** kullanılarak LED pininin durumu periyodik olarak değiştirilmektedir. Bu işlemin başlatılabilmesi için öncelikle GPIO'nun doğru şekilde yapılandırılması gerekmektedir. `SYS_INIT` sayesinde **GPIO yapılandırma** fonksiyonu sistem başlatıldığında otomatik olarak çağrılır.

```c
void thread_function(void* vp1, void* vp2 ,void* vp3)
{
        ARG_UNUSED(vp1);
        ARG_UNUSED(vp2);
        ARG_UNUSED(vp3);
        while(1)
        {
                gpio_pin_toggle_dt(&led);
                k_msleep(THREAD_SLEEP_TIME);
        }
}

K_THREAD_DEFINE(thread_id, STACK_SIZE, thread_function, NULL, NULL, NULL,
        THREAD_PRIORITY, 0, 0);
```

Bu kodda, bir **thread** oluşturulmuş ve her 1000 milisaniyede bir LED'in durumu değiştirilmiştir. Bu işlem, `gpio_init` fonksiyonu başarılı bir şekilde çağrıldıktan sonra başlar.

---

