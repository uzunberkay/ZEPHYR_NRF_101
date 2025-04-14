
---

## 🧵 Zephyr Thread 

**Zephyr RTOS** üzerinde birden fazla görev (task) aynı anda çalıştırmak için thread'ler kullanılır. Thread'ler, tek çekirdekli sistemlerde bile işlemciyi zaman dilimlerine bölerek farklı görevlerin paralel gibi çalışmasını sağlar. Bu örnekte, **Zephyr RTOS** üzerinde thread oluşturma ve kullanma süreci kısaca açıklanacaktır.

---

## 🧾 1. Thread Nedir?

**Thread**, bir programın çalıştırılabilen en küçük birimidir ve bir işlem içinde bağımsız olarak çalışabilen bir yol olarak tanımlanabilir. Tek çekirdekli sistemlerde bile thread'ler, işlemciyi belirli zaman dilimlerine bölerek farklı görevlerin paralel olarak çalışıyormuş gibi görünmesini sağlar. **Zephyr RTOS**, çoklu thread'ler oluşturmanıza ve her biri için öncelik ve zamanlama yönetimi sağlamanıza olanak tanır. Thread'ler, daha verimli bir işlem yönetimi ve zaman paylaşımlı işleme sağlar.

---

## 🔧 2. Thread Tanımlamak

Zephyr'de thread tanımlamak için `K_THREAD_DEFINE()` makrosu kullanılır. Bu makro, thread fonksiyonunu ve diğer parametreleri alır. Aşağıda thread tanımlaması örneği verilmiştir:

```c
K_THREAD_DEFINE(thread_id, THREAD_STACK_SIZE, thread_func, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);
```

### Parametreler:
- `thread_id`: Thread'e verilecek benzersiz kimlik.
- `THREAD_STACK_SIZE`: Thread için ayrılacak yığın (stack) boyutu.
- `thread_func`: Thread'in çalıştıracağı fonksiyon.
- `THREAD_PRIORITY`: Thread'in işlem sırasındaki önceliği (düşük sayılar yüksek önceliği ifade eder).

---

## ⚙️ 3. Thread Fonksiyonu

Thread fonksiyonu, thread'in gerçekleştireceği işlemi tanımlar. Aşağıda bir örnek thread fonksiyonu verilmiştir:

```c
void thread_func(void* p1, void* p2, void* p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    while (1) {
        // İşlem yapılacak kod
        k_sleep(K_MSEC(1000));  // 1 saniye boyunca thread hiçbir işlem yapmaz
    }
}
```

Thread fonksiyonu, bir sonsuz döngü içinde sürekli olarak belirli işlemleri gerçekleştirir. `k_sleep()` fonksiyonu, belirli bir süre boyunca thread'in beklemesini sağlar.

---

## 🔁 4. Thread Kullanımı

Thread'in kullanımı, Zephyr RTOS üzerinde paralel işlemleri yönetmek için oldukça etkilidir. Thread oluşturulduğunda, her bir thread bağımsız olarak çalışır. Aşağıda bir örnek kullanım gösterilmektedir:

```c
K_THREAD_DEFINE(thread1_id, THREAD_STACK_SIZE, thread1_func, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);
```

Bu satırda, `thread1_func` fonksiyonu ile bir thread tanımlanır ve belirli bir yığın boyutu ve öncelik ile çalıştırılır.

---

## 🧠 5. Thread Yönetimi

Thread'ler, **yığın (stack)** ve **öncelik (priority)** ile yönetilir. Zephyr RTOS, thread'ler arasında zaman dilimlendirmeyi yaparak verimli bir işlem yönetimi sağlar. Thread öncelikleri, işlem sırasını ve hangi thread'in ne zaman çalışacağını belirler.

---

