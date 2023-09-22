#include "include/network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// define the socket
int sock;

// Signal handler function
void handleCtrlC(int signal) {
    if (sock >= 0) {
        close(sock);
    }
    printf("\nExiting gracefully.\n");
    exit(signal);
}


int main() {
    signal(SIGINT, handleCtrlC);

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
    }

    // Set up the address structure to bind to the broadcast port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the broadcast port
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Binding error");
        close(sock);
        exit(1);
    }
    printf("Receiver node listening...\n");
    printf(" -Port: %d (%d)\n", PORT, htons(PORT));
    printf("===============================\n");

    char message[MAX_MESSAGE_SIZE];
    while (1) {
        ssize_t received_bytes = recvfrom(sock, message, sizeof(message), 0, NULL, NULL);
        if (received_bytes < 0) {
            perror("Message receiving error");
            close(sock);
        }
        message[received_bytes] = '\0'; // Null-terminate the received message
        printf(" - Message: %s\n", message);
    }

    close(sock);
    return 0;
}