#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/hmac.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SECRET_KEY "supersecretkey"

void calculate_hmac_sha256(const char *data, size_t data_len, unsigned char *result) {
    unsigned int len = 32;
    HMAC(EVP_sha256(), SECRET_KEY, strlen(SECRET_KEY), (unsigned char *)data, data_len, result, &len);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    unsigned char hash[32];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket oluşturulamadı.\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "192.168.1.10", &serv_addr.sin_addr) <= 0) {
        printf("Geçersiz adres.\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Bağlantı hatası.\n");
        return -1;
    }

    FILE *file_to_send = fopen("dosya.txt", "rb");
    if (file_to_send == NULL) {
        perror("Dosya açılamadı");
        close(sock);
        return -1;
    }
    fseek(file_to_send, 0, SEEK_END);
    long file_size = ftell(file_to_send);
    fseek(file_to_send, 0, SEEK_SET);

    char *file_content = malloc(file_size);
    fread(file_content, 1, file_size, file_to_send);
    calculate_hmac_sha256(file_content, file_size, hash);
    free(file_content);

    send(sock, hash, 32, 0);
    printf("HMAC-SHA256 hash'i gönderildi.\n");

    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file_to_send)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    printf("Dosya gönderildi.\n");

    fclose(file_to_send);
    close(sock);
    return 0;
}
