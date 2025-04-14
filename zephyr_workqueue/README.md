
---

# ⚡ Zephyr Workqueue 

## 🔄 WorkQueue Nedir?
Zephyr RTOS’ta **workqueue**, zaman alan veya ertelenmiş işlemleri thread dışında arka planda çalıştırmak için kullanılan bir yapıdır. Bu sistem sayesinde zamanlayıcı gibi kesme tabanlı olaylar içinde doğrudan uzun işlemler yapılmaz, onun yerine bu işlemler bir work objesi olarak **kuyruğa alınır** ve uygun zamanda çalıştırılır.

Bu projede **Zephyr RTOS** üzerinde bir **workqueue** kullanarak, zamanlayıcı tetiklemeleriyle LED'in düzenli aralıklarla yanıp sönmesini sağlıyoruz. Timer her tetiklendiğinde, bir **work** (iş) **workqueue**'ye ekleniyor ve arka planda LED'in durumu güncelleniyor. Bu sayede, LED'in blink işlemi, interrupt servis rutininde (ISR) doğrudan yapılmak yerine güvenli bir şekilde workqueue kullanılarak işleniyor. Böylece sistemin ana işleyişi bozulmadan zaman uyumsuz işlemler arka planda **yürütülebilir** hale geliyor.

## 🧾 1. prj.conf (KConfig) Dosyası

Projede, **workqueue** fonksiyonlarının düzgün çalışabilmesi için `prj.conf` dosyasına ek bir şey yazılmasına gerek yoktur. Zephyr, **workqueue** desteğini varsayılan olarak sağlar.

## 🗺️ 2. DeviceTree (Overlay) Dosyasıı

Bu projede, **workqueue** için özel bir yapılandırma veya DeviceTree (overlay) dosyasına ek bir şey yazılmasına gerek yoktur. İlgili cihaz ve GPIO tanımlamaları **gpio_driver.h** dosyasından yapılmıştır.

## 📋 3. Workqueue Kullanımı

### **Workqueue Tanımlama**

```c
K_WORK_DEFINE(workQ_led_blink, workH_led_blink);
```

- **`K_WORK_DEFINE`**: Bu makro, bir **work** (iş) tanımlar. `workQ_led_blink`, işin adıdır, `workH_led_blink` ise işin yerine getireceği fonksiyondur. Bu iş, LED'in durumunu değiştiren bir işlemi gerçekleştirecektir.

### **Workqueue Başlatma**

```c
k_work_queue_init(&wq);
k_work_queue_start(&wq, wq_stack, WORKQUEUE_STACK_SIZE, WORKQUEUE_PRIORITY, NULL);
```

- **`k_work_queue_init`**: Bu fonksiyon, **workqueue**'yi başlatır.
- **`k_work_queue_start`**: Bu fonksiyon, **workqueue**'yi çalıştırmaya başlar. `wq_stack` stack belleğini, `WORKQUEUE_STACK_SIZE` stack boyutunu, ve `WORKQUEUE_PRIORITY` işlerin önceliğini belirtir.

### **Workqueue Handler Fonksiyonu**

```c
void workH_led_blink(struct k_work* work)
{
    LOG_DBG("LED Blink");
    gpio_pin_toggle_dt(&led);
}
```

- **`workH_led_blink`**: Bu fonksiyon, **workqueue**'den gelen her iş tetiklendiğinde çalışacak olan fonksiyondur. Bu örnekte, LED'in durumu değiştirilir (yanıp söner).

### **Timer ve Workqueue Entegrasyonu**

```c
void timer_handler(struct k_timer *dummy)
{
    LOG_DBG("Timer Handler");

    if(k_work_submit_to_queue(&wq, &workQ_led_blink) < 0)
    {
        LOG_ERR("workQ_led_blink submit failed");
        return;
    } 

    LOG_DBG("workQ_led_blink submitted");
}
```

- **`timer_handler`**: Zamanlayıcı tetiklendiğinde çağrılan fonksiyondur. Burada, **workqueue**'ye `workQ_led_blink` işini ekleriz. Bu iş, **workqueue**'deki işleyici tarafından işleme alınır ve LED'in durumu değiştirilir.

## 📝 Notlar

- **Workqueue**: **Zephyr RTOS**, asenkron işlerin yönetimi için **workqueue** fonksiyonlarını sağlar. Bu proje, **timer** ve **workqueue**'yi birleştirerek LED'in her saniye yanıp sönmesini sağlar. Timer, belirli aralıklarla **workqueue**'ye iş ekler ve bu işler **workqueue**'deki işleyiciler tarafından sırayla işlenir.
  
- **Zamanlayıcı (Timer)**: Zamanlayıcı, belirli bir süre sonunda bir işlem başlatan bir işlevdir. Burada, **timer** her saniye tetiklenir ve **workqueue**'ye bir iş ekler. Bu iş de **workqueue handler** fonksiyonu ile işlenir.

---

