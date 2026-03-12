#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Düðüm (Node) Yapýsý
typedef struct LogNode {
    char message[MAX_LINE_LENGTH];
    struct LogNode* next;
} LogNode;

// Yeni düðüm oluþturma
LogNode* create_node(const char* message) {
    LogNode* new_node = (LogNode*)malloc(sizeof(LogNode));
    if (new_node == NULL) {
        perror("Bellek ayirma hatasi");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->message, message, MAX_LINE_LENGTH - 1);
    new_node->message[MAX_LINE_LENGTH - 1] = '\0'; 
    new_node->next = NULL;
    return new_node;
}

// Listeye ekleme
void append_log(LogNode** head, const char* message) {
    LogNode* new_node = create_node(message);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    LogNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}

// Ekrana yazdýrma
void print_logs(LogNode* head) {
    LogNode* temp = head;
    int count = 1;
    printf("\n--- Kaydedilen Sistem Gunlukleri ---\n");
    while (temp != NULL) {
        printf("[%d] %s", count, temp->message);
        temp = temp->next;
        count++;
    }
    printf("------------------------------------\n");
}

// Bellek temizleme
void free_logs(LogNode* head) {
    LogNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// --- DÜZELTME: Otomatik Test Dosyasý Oluþturan Fonksiyon ---
void create_dummy_syslog() {
    FILE* file = fopen("test_syslog.txt", "w");
    if (file != NULL) {
        fprintf(file, "Oct 24 10:45:01 server sshd[1234]: Accepted publickey for root\n");
        fprintf(file, "Oct 24 10:45:05 server kernel: [ 12.345] usb 1-1: new high-speed USB device\n");
        fprintf(file, "Oct 24 10:46:00 server cron[5678]: (root) CMD ( /usr/bin/task)\n");
        fprintf(file, "Oct 24 10:48:12 server sudo[9012]: user : TTY=pts/0 ; COMMAND=/bin/su\n");
        fclose(file);
    }
}

int main() {
    // 1. ADIM: Hata almamak için önce test dosyasýný otomatik oluþturuyoruz
    create_dummy_syslog();

    LogNode* log_list_head = NULL; 
    char buffer[MAX_LINE_LENGTH];
    
    // 2. ADIM: Oluþturduðumuz dosyayý okuma modunda açýyoruz
    FILE* file = fopen("test_syslog.txt", "r");
    if (file == NULL) {
        perror("Beklenmeyen bir hata olustu");
        return EXIT_FAILURE;
    }

    // Dosyayý satýr satýr oku ve listeye ekle
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        append_log(&log_list_head, buffer);
    }

    fclose(file);

    // Ýþlenmiþ veriyi göster
    print_logs(log_list_head);

    // Belleði temizle
    free_logs(log_list_head);

    return EXIT_SUCCESS;
}
