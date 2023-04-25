#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include "mqtt_connect.h"


int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd,ip_fd;
	char* server_ip = "127.0.0.1";
	struct sockaddr_in serv_addr;
	struct ifreq ifr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	char buffer2[1024] = { 0 };
	struct mqtt_connect con;
	con.fixed_header[0] = 0b00010000;
	con.fixed_header[1] = 0b00000000;
	con.variable_header[0] = 0b0;
	con.variable_header[1] = 0b0;
	con.variable_header[2] = 'M';
	con.variable_header[3] = 'Q';
	con.variable_header[4] = 'T';
	con.variable_header[5] = 'T';
	con.variable_header[6] = 0x04;
	con.variable_header[7] = 0b0;
	for (int i = 0; i < 22; i++){
		con.client_identifier[i] = 0b0;
	}
	con.client_identifier[22] = 0b1;
	con.will_message = "";
	con.user_name = "test";
	con.password = "1234";
	con.keep_alive[0] = 0b0;
	con.keep_alive[0] = 0b1;	

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	char * message = "";
	message += con.fixed_header[0];
	message += con.fixed_header[1];
	for (int i = 0; i < 8; i++)
	{
		message += con.variable_header[i];
	}
	for (int i = 0; i < 23; i++)
	{
		message += con.client_identifier[i];
	}
	message += con.user_name;
	message += con.password;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(7001);

	if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	write(sock,message,strlen(message));
	read(sock, buffer,1024);
	printf("%x",buffer);
	// closing the connected socket
	close(client_fd);
	return 0;
}
