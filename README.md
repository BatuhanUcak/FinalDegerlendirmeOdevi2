C ile Bağlı Liste (Bağlantılı Liste) Kullanarak Syslog Yönetimi
Proje, sistem günlüklerini (syslog) simüle eden bu verileri bellekte Tek Yönlü Bağlı Liste (Tek Yönlü Bağlı Liste) veri yapısını kullanarak dinamik bir şekilde saklayan bir C uygulamasıdır.

Sistem günlükleri, boyutları bilinemeyen ve sürekli artan veriler olduğu için, bu proje statik diziler (diziler) yerine dinamik bellek dağıtımı (dinamik bellek ayırma) ile verimli bir bellek yönetimi hedeflenmiştir.

🚀 Özellikler
Otomatik Test Verisi: Çalıştırıldığında hata almamak için kendi test_syslog.txtdosyaları otomatik olarak oluşturulur ve içine örnek log kayıtları yazar.

Dinamik Bellek Yönetimi: Her bir log kaydı için yalnızca gerekli kadar bellek bölünmesi ( malloc) ve islem bitiminde sızıntıları (bellek sızıntısı) önlenmesi için belleklerin kesilmesi ( free).

Dosya İşlemleri: Dosyadan satır satır okuma işleminin verimine bağlı olarak listeye düğüm (node) olarak eklenir.

🛠️ Kullanılan Yapılar ve Fonksiyonlar
struct LogNode: Log mesajını ve bir sonraki düğümün adresini tutan temel veri yapısını.

create_dummy_syslog(): Örnek bir günlük dosyası oluşturulur.

create_node(): Gelen mesaj için bellekte yeni bir alan açılır.

append_log(): Yeni düğüm düğümüne bağlı dinleme sonuna kadar tamamlanır.

print_logs(): Bağlı liste üzerinde gezinerek tüm logları formatlı bir şekilde yazdırılır.

free_logs(): Program kapanmadan bir kez tahsis edilen tüm bellek sistemine geri gönderilir.

💻 Nasıl Çalıştırılır?
Projeyi kendi hızında derleyip çalıştırmak için terminalinizde aşağıdaki adımları izleyin.

1. Kodu Derleyin:

Bash
gcc main.c -o syslog_manager
2. Programının Hazırlanması:

Bash
./syslog_manager
📋 Örnek Çıktı
Programı çalıştırdığınızda konsolda aşağıdaki gibi bir çıktı görmeniz gerekir:

Düz metin
--- Kaydedilen Sistem Gunlukleri ---


[1] Oct 24 10:45:01 server sshd[1234]: Accepted publickey for root


[2] Oct 24 10:45:05 server kernel: [ 12.345] usb 1-1: new high-speed USB device


[3] Oct 24 10:46:00 server cron[5678]: (root) CMD ( /usr/bin/task)


[4] Oct 24 10:48:12 server sudo[9012]: user : TTY=pts/0 ; COMMAND=/bin/su

------------------------------------
