#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "mqtt_connack.h"

void *thread_server(void *vargp)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char ip[1024] = {0};
    char message[1024] = {0};
    char hand[2] = {0};
    int valread;
    char *hello = "Hello from server";
    struct mqtt_connack con;
    con.fixed_header[0] = 0b00100000;
    con.fixed_header[1] = 0b00000010;
    con.variable_header[0] = 0b0;
    con.variable_header[0] = 0b0;
    
    char * message2 = "";
    message2 += con.fixed_header[0];
    message2 += con.fixed_header[1];
    message2 += con.variable_header[0];
    message2 += con.variable_header[1];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7001);
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        if (listen(server_fd, 2) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("Connection Error");
            close(new_socket);
        }
        read(new_socket, message, 1024);
        printf("%x", (int) message);
        write(new_socket,&con,4);
        close(new_socket);
    }
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread;

    pthread_create(&thread, NULL, thread_server, NULL);
    pthread_join(thread, NULL);

    return 0;
}
