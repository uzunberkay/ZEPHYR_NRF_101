
---

# ⚡ Zephyr Timer

Bu proje, Zephyr RTOS üzerinde çalışan bir geliştirme kartı kullanarak bir LED'in her saniye yanıp sönmesini sağlayan basit bir timer uygulamasıdır. Zephyr'in zamanlayıcı (timer) fonksiyonları kullanılarak belirli bir zaman aralığında işlemler yapılabilir. Bu örnekte, timer her saniye tetiklenerek bir LED'in durumunu değiştirir.

## 🧾 1. prj.conf (KConfig) Dosyası

Projede, zamanlayıcı fonksiyonlarının düzgün çalışabilmesi için `prj.conf` dosyasına ekstra bir şey eklenmesine gerek yoktur. Zephyr, zamanlayıcı desteğini varsayılan olarak sağlar.

## 🗺️ 2. DeviceTree (Overlay) Dosyası

Bu projede, timer kullanımı için özel bir yapılandırma veya DeviceTree (overlay) dosyasına ek bir şey yazılmasına gerek yoktur. Zamanlayıcılar, yazılım seviyesinde doğrudan Zephyr fonksiyonlarıyla yönetilir.

## 📋 3. Timer Kullanımı

### Timer Tanımlama

```c
K_TIMER_DEFINE(sample_timer, timer_handler, NULL);
```

- **`K_TIMER_DEFINE`**: Bu makro, bir zamanlayıcıyı tanımlar. Zamanlayıcı adı (`sample_timer`), her tetiklenme sonrası çalışacak fonksiyon (`timer_handler`) ve durdurulduğunda çalışacak fonksiyon (`NULL`) belirtilir.
- Zamanlayıcı, her tetiklendiğinde `timer_handler` fonksiyonunu çalıştıracaktır.

### Timer Başlatma

```c
k_timer_start(&sample_timer, K_MSEC(1000), K_MSEC(1000));
```

- **`k_timer_start`**: Zamanlayıcıyı başlatmak için kullanılır. İlk parametre, başlatılacak zamanlayıcının adresini alır. İkinci parametre, zamanlayıcının ilk tetiklenme süresini (1000 ms = 1 saniye) belirler. Üçüncü parametre ise zamanlayıcının her tetiklenme arasındaki periyodu (1000 ms = her saniye tetiklenecek) belirler.

### Timer Handler Fonksiyonu

```c
void timer_handler(struct k_timer *dummy)
{
    gpio_pin_toggle_dt(&led);
}
```

- **`timer_handler`**: Zamanlayıcı her tetiklendiğinde çalışacak fonksiyondur. Burada, LED'in durumu değiştirilir (yanıp söner).

## 📝 Notlar

- **Zamanlayıcı**: Zamanlayıcı, Zephyr RTOS'un sunduğu yerleşik fonksiyonlarla kolayca yönetilebilir. `K_TIMER_DEFINE` ile tanımlanır, `k_timer_start` ile başlatılır ve her tetiklenme sonrası belirtilen handler fonksiyonu çalıştırılır.

---
