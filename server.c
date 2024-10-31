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
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    unsigned char received_hash[32], calculated_hash[32];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind hatası");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Dinleme hatası");
        exit(EXIT_FAILURE);
    }
    printf("Server dinlemede...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Bağlantı kabul hatası");
        exit(EXIT_FAILURE);
    }

    read(new_socket, received_hash, 32);
    printf("HMAC-SHA256 hash'i alındı.\n");

    FILE *received_file = fopen("dosya.txt", "wb");
    if (received_file == NULL) {
        perror("Dosya açılamadı");
        exit(EXIT_FAILURE);
    }

    int bytes_received;
    char *file_content = malloc(BUFFER_SIZE);
    size_t total_bytes = 0;

    while ((bytes_received = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, 1, bytes_received, received_file);
        memcpy(file_content + total_bytes, buffer, bytes_received);
        total_bytes += bytes_received;
    }

    calculate_hmac_sha256(file_content, total_bytes, calculated_hash);
    if (memcmp(received_hash, calculated_hash, 32) == 0) {
        printf("Dosya başarıyla doğrulandı ve kaydedildi.\n");
    } else {
        printf("Dosya doğrulama başarısız!\n");
    }

    free(file_content);
    fclose(received_file);
    close(new_socket);
    close(server_fd);

    return 0;
}
