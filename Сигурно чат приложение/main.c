#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 16384 // Увеличен буфер за по-големи RSA ключове

int main() {
    int server_fd, client1, client2;
    struct sockaddr_in address;
    int opt = 1, addrlen = sizeof(address);
    unsigned char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    listen(server_fd, 3);

    printf("Server E2EE Relay started on port %d\n", PORT);

    // === [BLOCK_CONN_ESTABLISH] ===
    client1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client A connected.\n");

    client2 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client B connected. Start relaying...\n");

    // === [BLOCK_RELAY_LOOP] ===
    fd_set readfds;
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(client1, &readfds);
        FD_SET(client2, &readfds);
        int max_sd = (client1 > client2) ? client1 : client2;

        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0) break;

        if (FD_ISSET(client1, &readfds)) {
            int valread = recv(client1, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) { printf("Client A disconnected.\n"); break; }
            send(client2, buffer, valread, 0);
            printf("Relay: A -> B (%d bytes)\n", valread);
        }

        if (FD_ISSET(client2, &readfds)) {
            int valread = recv(client2, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) { printf("Client B disconnected.\n"); break; }
            send(client1, buffer, valread, 0);
            printf("Relay: B -> A (%d bytes)\n", valread);
        }
    }

    close(client1);
    close(client2);
    close(server_fd);
    return 0;
}
