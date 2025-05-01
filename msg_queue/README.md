
---

## 📬 Zephyr Message Queue (k_msgq) Örneği

Bu örnek proje, **Zephyr RTOS** üzerinde `k_msgq` API'sini kullanarak iki thread arasında veri paylaşımını göstermektedir.  
Mesaj kuyruğu (`k_msgq`), thread'ler arasında **senkronize ve güvenli veri iletimini sağlamak** için kullanılır.

Bir üretici (producer) thread sahte sensör verileri üretir ve bu verileri mesaj kuyruğuna ekler.  
Bir tüketici (consumer) thread ise sırayla bu verileri kuyruğundan çekerek işler.

---

### 📁 Dosya Yapısı

```txt
src/
├── main.c             --> Ana uygulama dosyası
CMakeLists.txt         --> CMake yapılandırması
prj.conf               --> Proje yapılandırma ayarları
```

---

### ⚙️ prj.conf

```ini
CONFIG_LOG=y
```

> Bu ayarlar log sistemini etkinleştirir .

---

### 📌 Kullanılan Yapılar

| Yapı                      | Açıklama                                                                 |
|---------------------------|--------------------------------------------------------------------------|
| `k_msgq`                  | Zephyr mesaj kuyruğu, thread'ler arası veri paylaşımında kullanılır.     |
| `k_msgq_put`              | Kuyruğa mesaj eklemek için kullanılır.                                   |
| `k_msgq_get`              | Kuyruktan mesaj almak için kullanılır.                                   |
| `k_msgq_num_used_get`     | Kuyruktaki mevcut mesaj sayısını öğrenmek için kullanılır.               |

---

### 🧵 Thread Detayları

| Thread              | Öncelik | Stack Boyutu | Görev                                         |
|---------------------|---------|--------------|-----------------------------------------------|
| `producer_thread`   | 7       | 1024 bytes   | Sahte sensör verisi üretir ve kuyruğa ekler.  |
| `consumer_thread`   | 8       | 1024 bytes   | Kuyruktan veri alır ve terminale log yazar.   |

---

### 🔄 Thread'ler Arası Veri Paylaşımı

Bu örnekte `k_msgq`, farklı thread'lerin güvenli bir şekilde **ortak veri alışverişi** yapmasını sağlar.  
Thread'ler arasında doğrudan global değişken kullanımı yerine, **eşzamanlılık sorunlarını önlemek** için mesaj kuyruğu kullanılır.

---

### 📊 Log Örnekleri

```sh
[00:00:01.001,000] <inf> Sensor verisi kuyruğa eklendi: 42
[00:00:01.101,000] <inf> Kuyruktan veri alindi: 42
```

---

### 🧠 Öğrenilecek Konular

✅ Zephyr'de `k_msgq` kullanımı  
✅ Thread senkronizasyonu ve veri paylaşımı  
✅ Kuyruk doluluk/boşluk durumuna göre zaman aşımı yönetimi  

---

### 📝 Notlar

- Bu örnek, gerçek bir sensör yerine `k_uptime_get_32()` ile üretilen sahte verileri kullanır.
- `k_msgq_put` ve `k_msgq_get`, belirli bir süre (timeout) boyunca bekleyebilir. Bu sayede sistem, kilitlenmeden çalışmaya devam eder.
- Kuyrukta yer yoksa `put` işlemi başarısız olur; veri alınamazsa `get` işlemi zaman aşımına uğrar.

---



