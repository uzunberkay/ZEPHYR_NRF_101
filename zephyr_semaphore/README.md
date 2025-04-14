
---

# 🟢 Zephyr Semaphore Kullanımı

## Semaphore Nedir ? 

Semaphore, iş parçacıkları arasında senkronizasyon sağlamak için kullanılan bir mekanizmadır. Zephyr'de, **k_sem_give()** semaforu "verirken", **k_sem_take()** semaforu bekler. Bu, kesintili işlemlerle (ISR veya zamanlayıcılar gibi) güvenli bir zamanlama yapmayı sağlar.

Neden Kullanılır?

- Zamanlama kontrolü sağlar.
- İş parçacıkları arasındaki iletişimi senkronize eder.
- Race condition gibi hataları önler.

Bu projede **Zephyr RTOS** üzerinde bir **semafor (semaphore)** kullanarak, bir **timer callback** fonksiyonunun bir **thread**'i tetiklemesini sağlıyoruz. Timer her tetiklendiğinde semafor "veriliyor" ve thread bu sinyali alarak LED'in durumunu değiştiriyor. Bu yapı, **asenkron** olayları senkron şekilde ve **güvenli bir zamanlamayla** yönetmek için ideal bir yöntem sunar.

## 🧾 1. prj.conf (KConfig) Dosyası

Bu örnekte, `prj.conf` dosyasına ek bir şey yazılmasına gerek yoktur. Zephyr, **k_sem** (kernel semaphore) desteğini varsayılan olarak sağlar.

## 🗺️ 2. DeviceTree (Overlay) Dosyası

Bu projede, **semaphore** için özel bir yapılandırma veya DeviceTree (overlay) dosyasına ek bir şey yazılmasına gerek yoktur

## 📌 3. Semaphore Kullanımı

### 📌 Semafor Tanımlama

```c
K_SEM_DEFINE(sample_sem, 0, 1);
```

- **`K_SEM_DEFINE`**: Bir semafor nesnesi tanımlar.
  - Başlangıç değeri: `0` (ilk başta thread bekler durumda).
  - Maksimum değeri: `1` (binary semaphore gibi davranır).

### 📌 Timer Callback ve Semafor Verme

```c
void timer_cb(struct k_timer* dummy)
{
    LOG_DBG("Timer callback");
    k_sem_give(&sample_sem);
}
```

- **`k_sem_give`**: Semafor değerini artırır, yani bir **izin** verir.
- Timer her tetiklendiğinde bu fonksiyon çağrılır.

### 📌 Thread ve Semafor Bekleme

```c
void sample_thread(void* vp1, void* vp2, void* vp3)
{
    while(true)
    {
        k_sem_take(&sample_sem, K_FOREVER);
        gpio_pin_toggle_dt(&led);
    }
}
```

- **`k_sem_take`**: Semaforu bekler. Eğer semafor değeri sıfırsa, bu fonksiyon **bloklanır**.
- `K_FOREVER`: Sonsuza kadar bekler.
- Semafor "verildiğinde", thread çalışır ve LED'in durumunu değiştirir.



---
