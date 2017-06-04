//
//  Server.cpp
//  FTP
//
//  Created by Adam Olsson on 2017-06-03.
//  Copyright © 2017 Adam Olsson. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
    int sockfd, portnumber, newsockfd, n;
    struct sockaddr_in serv_address, cli_address;
    char buffer[256];

    if(argcount < 2)
    {
        fprintf(stderr, "Error, no port provided");
        exit(1); //could make user enter a new port
    }

    /*
     AF_NET: Addess domain is of type Internet Domain
     SOCK_STREAM: The type of socked is Stream
     0: What protocol? The OS will choose most appropriate protovol
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //sockfd = Socket File Describtor
    if(sockfd < 0) // if no socket created
    {
        fprintf(stderr, "Socket not created");
        exit(1);
    }

    bzero((char *) &serv_address, sizeof(serv_address));

    portnumber = atoi(argv[1]); //Convert from string to int
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(portnumber); //Convert portnumber to network byte order, from little endian to big endian
    serv_address.sin_addr.s_addr = INADDR_ANY; // IP of host. This will always be IP of machine but this value is just symbolic

    if(bind(sockfd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0)
    {
        fprintf(stderr, "Error, could not bind socket");
        exit(1);
    }
    listen(sockfd, 5);

    socklen_t clilen = sizeof(cli_address);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_address, &clilen); // Halt until someone connects
    if(newsockfd < 0)
    {
        perror("Error on accept"); // Ska vara konsekvent med error men vet inte vilket jag ska använda
    }

    bzero(buffer, 256); //sizeof(buffer)? //initialize the buffer

    n = read(newsockfd, buffer, 255); // why 255 and not 256? pga av att den börjar på samma minnesadress som pointern
                                      // Read will block until client has executed write()

    if(n < 0)
    {
        perror("Error, writing to socket")
    }

    n = write(newsockfd, "I got your message", 18);

    return 0;
}
