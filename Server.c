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

int main() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("WSAStartup failed\n");
            return 1;
        }
    #endif

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        #ifdef _WIN32
            WSACleanup();
        #endif
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port
    #ifdef _WIN32
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt))) {
    #else
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    #endif
        perror("setsockopt");
        #ifdef _WIN32
            closesocket(server_fd);
            WSACleanup();
        #else
            close(server_fd);
        #endif
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        #ifdef _WIN32
            closesocket(server_fd);
            WSACleanup();
        #else
            close(server_fd);
        #endif
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        #ifdef _WIN32
            closesocket(server_fd);
            WSACleanup();
        #else
            close(server_fd);
        #endif
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    while(1) {
        printf("Waiting for a connection...\n");
        
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            #ifdef _WIN32
                closesocket(server_fd);
                WSACleanup();
            #else
                close(server_fd);
            #endif
            exit(EXIT_FAILURE);
        }
        
        printf("Connection accepted\n");
        
        // Read client message
        #ifdef _WIN32
            int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
        #else
            int valread = read(new_socket, buffer, BUFFER_SIZE);
        #endif
        printf("Message from client: %s\n", buffer);
        
        // Send response back to client
        char *response = "Hello from server";
        send(new_socket, response, strlen(response), 0);
        printf("Response sent to client\n");
        
        #ifdef _WIN32
            closesocket(new_socket);
        #else
            close(new_socket);
        #endif
    }
    
    #ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
    #else
        close(server_fd);
    #endif
    
    return 0;
}