#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("WSAStartup failed\n");
            return 1;
        }
    #endif

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        #ifdef _WIN32
            WSACleanup();
        #endif
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (argc < 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        #ifdef _WIN32
            closesocket(sock);
            WSACleanup();
        #else
            close(sock);
        #endif
        return -1;
    }
    
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        #ifdef _WIN32
            closesocket(sock);
            WSACleanup();
        #else
            close(sock);
        #endif
        return -1;
    }
    
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        #ifdef _WIN32
            closesocket(sock);
            WSACleanup();
        #else
            close(sock);
        #endif
        return -1;
    }
    
    // Send message to server
    char *message = "Hello from client";
    send(sock, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);
    
    // Receive response from server
    #ifdef _WIN32
        int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    #else
        int valread = read(sock, buffer, BUFFER_SIZE);
    #endif
    printf("Response from server: %s\n", buffer);
    
    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif
    
    return 0;
}