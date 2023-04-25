#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd,ip_fd;
	char* server_ip = "127.0.0.1";
	struct sockaddr_in serv_addr;
	struct ifreq ifr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	char buffer2[1024] = { 0 };

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

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
	char* x = "AA";
	write(sock, x , strlen(x));
	read(sock, buffer,1024);
	if (strcmp(buffer,"AA") == 0){
		ifr.ifr_addr.sa_family = AF_INET;
		strncpy(ifr.ifr_name, "lo", IFNAMSIZ-1);
		ioctl(ip_fd, SIOCGIFADDR, &ifr);
		close(ip_fd);
		//char* ip_client = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr); la ip me estaba regresando 0.0.0.0 del host
		char* ip_client = "192.168.1.2, 192.168.1.1";
		send(sock,ip_client,strlen(ip_client),0);
		sleep(1);
		send(sock,hello, strlen(hello),0);
		read(sock,buffer,1024);
    	read(sock,buffer2,1024);
    	printf("%s, ",buffer);
    	printf("%s",buffer2);

	}
	// closing the connected socket
	close(client_fd);
	return 0;
}
