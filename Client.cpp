#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char* msg)
{
    perror(msg);
    printf("\n");
    exit(1);
}

int main(int argc, char* argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buffer[256];

    if(argc < 3) //kontrollerar antalet args är korrekt
    {
        fprintf(stderr, "Usage %s hostname port.\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening socket.\n");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL)
    {
        fprintf(stderr, "Error, no such host.\n");
        exit(0);
    }

    printf("Server found. Connecting... \n");

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char*)server->h_addr_list, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error connecting");
    }

    printf("Connected. \n");
    printf("Please enter message: \n");
    
    memset(buffer, 0, 256);
    fgets(buffer, 255, stdin);

    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)
    {
        error("Error writing socket. \n");
    }

    memset(buffer, 0, 256);
    
    n = read(sockfd, buffer, 255);
    if(n < 0)
    {
        error("Error reading from socket. \n");
    }

    printf("%s \n", buffer);
    close(sockfd);
    return 0;

}