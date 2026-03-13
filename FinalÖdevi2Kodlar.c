#include <stdio.h>   // Giriþ/Çýkýþ iþlemleri (printf, fopen, fgets vb.) için gerekli temel kütüphane.
#include <stdlib.h>  // Dinamik bellek yönetimi (malloc, free, exit vb.) için gerekli kütüphane.
#include <string.h>  // Metin (string) kopyalama ve iþleme (strncpy vb.) iþlemleri için gerekli kütüphane.

#define MAX_LINE_LENGTH 1024 // Bir log satýrýnýn bellekte kaplayabileceði maksimum karakter sayýsýný belirliyoruz.

// Düðüm (Node) Yapýsý
typedef struct LogNode {
    char message[MAX_LINE_LENGTH]; // Log mesajýnýn metnini tutacak olan karakter dizisi (string).
    struct LogNode* next;          // Zincirin bir sonraki halkasýný (düðümünü) gösterecek iþaretçi (pointer).
} LogNode;

// Yeni düðüm oluþturma
LogNode* create_node(const char* message) {
    // 1. RAM'de (Heap bölgesinde) tam olarak bir LogNode boyutunda yer (bellek) ayýrýyoruz.
    LogNode* new_node = (LogNode*)malloc(sizeof(LogNode)); 
    
    // 2. Eðer bellek ayrýlamadýysa (RAM doluysa vb.) programý güvenli bir þekilde kapatýyoruz.
    if (new_node == NULL) {
        perror("Bellek ayirma hatasi"); // Ekrana sistemin hata mesajýný basar.
        exit(EXIT_FAILURE);             // Programý hata kodu (1) ile anýnda sonlandýrýr.
    }
    
    // 3. Gelen log mesajýný, düðümün içindeki message dizisine güvenli bir þekilde (sýnýrý aþmadan) kopyalýyoruz.
    strncpy(new_node->message, message, MAX_LINE_LENGTH - 1);
    
    // 4. String'in sonuna, C dilinde metnin bittiðini belirten "Null Terminator (\0)" karakterini manuel koyuyoruz.
    new_node->message[MAX_LINE_LENGTH - 1] = '\0'; 
    
    // 5. Bu yepyeni bir düðüm olduðu ve henüz listeye eklenmediði için next iþaretçisini NULL (boþ) yapýyoruz.
    new_node->next = NULL; 
    
    return new_node; // Baþarýyla oluþturduðumuz bu yeni düðümü geri döndürüyoruz.
}

// Listeye ekleme
void append_log(LogNode** head, const char* message) {
    LogNode* new_node = create_node(message); // Önce eklenecek log için bellekte yepyeni bir düðüm oluþturuyoruz.
    
    // Eðer liste þu an tamamen boþsa (*head iþaretçisi NULL ise):
    if (*head == NULL) {
        *head = new_node; // Yeni oluþturduðumuz düðümü listenin en baþý (ilk elemaný) yapýyoruz.
        return;           // Ekleme iþi bittiði için fonksiyondan çýkýyoruz.
    }
    
    // Liste boþ deðilse, en son elemaný bulmak için geçici bir iþaretçi (temp) oluþturuyoruz.
    LogNode* temp = *head; 
    
    // temp'in gösterdiði düðümün bir sonrasý (next) boþ (NULL) olana kadar listeyi ileri doðru geziyoruz.
    while (temp->next != NULL) {
        temp = temp->next; // Bir sonraki düðüme atla.
    }
    
    // Döngü bittiðinde temp artýk zincirin "son" düðümünü gösteriyor demektir. 
    // Son düðümün next'ine yeni oluþturduðumuz düðümü baðlýyoruz.
    temp->next = new_node; 
}

// Ekrana yazdýrma
void print_logs(LogNode* head) {
    LogNode* temp = head; // Listeyi baþtan sona gezmek için temp iþaretçisini baþlangýca (head) koyuyoruz.
    int count = 1;        // Loglarý 1, 2, 3 diye numaralandýrmak için bir sayaç baþlatýyoruz.
    
    printf("\n--- Kaydedilen Sistem Gunlukleri ---\n");
    
    // temp boþ (NULL) olana kadar, yani liste tamamen bitene kadar döngüyü çalýþtýrýyoruz.
    while (temp != NULL) {
        printf("[%d] %s", count, temp->message); // Sýra numarasýný ve düðümdeki log mesajýný ekrana basýyoruz.
        temp = temp->next;                       // Yazdýrma bitince bir sonraki düðüme geçiyoruz.
        count++;                                 // Sayacý bir artýrýyoruz.
    }
    printf("------------------------------------\n");
}

// Bellek temizleme
void free_logs(LogNode* head) {
    LogNode* temp; // Silinecek düðümü geçici olarak tutacaðýmýz iþaretçi.
    
    // Liste bitene kadar döngüyü çalýþtýrýyoruz.
    while (head != NULL) {
        temp = head;       // Þu an üzerinde olduðumuz düðümü temp'e alýyoruz.
        head = head->next; // head iþaretçisini bir sonraki düðüme kaydýrarak zinciri koparmamayý/kaybetmemeyi saðlýyoruz.
        free(temp);        // temp'in tuttuðu düðümü RAM'den tamamen siliyoruz (belleði iþletim sistemine iade ediyoruz).
    }
}

// --- Otomatik Test Dosyasý Oluþturan Fonksiyon ---
void create_dummy_syslog() {
    FILE* file = fopen("test_syslog.txt", "w"); // Dosyayý "Yazma" (Write - w) modunda açýyoruz. Yoksa oluþturulur.
    
    if (file != NULL) { // Dosya baþarýyla açýldýysa/oluþturulduysa:
        // Ýçine örnek (simülasyon) Linux Syslog mesajlarýný satýr satýr yazýyoruz.
        fprintf(file, "Oct 24 10:45:01 server sshd[1234]: Accepted publickey for root\n");
        fprintf(file, "Oct 24 10:45:05 server kernel: [ 12.345] usb 1-1: new high-speed USB device\n");
        fprintf(file, "Oct 24 10:46:00 server cron[5678]: (root) CMD ( /usr/bin/task)\n");
        fprintf(file, "Oct 24 10:48:12 server sudo[9012]: user : TTY=pts/0 ; COMMAND=/bin/su\n");
        fclose(file); // Ýþimiz bitince dosyayý kapatarak kilitlenmesini önlüyoruz.
    }
}

int main() {
    // 1. ADIM: Okuyacak bir þey bulamayýp hata almamak için önce test dosyamýzý otomatik oluþturuyoruz.
    create_dummy_syslog();

    LogNode* log_list_head = NULL;  // Baðlý listemizin baþlangýç noktasýný (head) þimdilik boþ (NULL) yapýyoruz.
    char buffer[MAX_LINE_LENGTH];   // Dosyadan okuyacaðýmýz her bir satýrý geçici olarak tutacaðýmýz metin tamponu (buffer).
    
    // 2. ADIM: Oluþturduðumuz log dosyasýný okuma modunda (Read - r) açýyoruz.
    FILE* file = fopen("test_syslog.txt", "r");
    
    if (file == NULL) { // Eðer dosya bulunamaz veya yetki hatasý yüzünden açýlamazsa:
        perror("Beklenmeyen bir hata olustu"); // Sistemin hata mesajýný basýyoruz.
        return EXIT_FAILURE;                   // Programý baþarýsýz kodla sonlandýrýyoruz.
    }

    // 3. ADIM: Dosyayý fgets ile satýr satýr okuyoruz. fgets, okunacak satýr kalmayýnca (EOF) NULL döndürür ve döngü biter.
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        append_log(&log_list_head, buffer); // Okuduðumuz o satýrý (buffer), baðlý listemizin sonuna yeni düðüm olarak ekliyoruz.
    }

    fclose(file); // Dosya ile iþimiz bitti, okumayý bitirdik ve dosyayý kapatýyoruz.

    // 4. ADIM: Ýþlenmiþ veriyi göster
    print_logs(log_list_head); // Bellekte düðümler halinde duran log zincirini sýrasýyla ekrana yazdýrýyoruz.

    // 5. ADIM: Belleði temizle (Memory Leak / Bellek Sýzýntýsý Önlemi)
    free_logs(log_list_head); // Program kapanmadan önce malloc ile ayýrdýðýmýz tüm bellek alanlarýný tek tek siliyoruz.

    return EXIT_SUCCESS; // Her þey planlandýðý gibi çalýþtý, programý iþletim sistemine baþarý (0) sinyali vererek bitiriyoruz.
}
