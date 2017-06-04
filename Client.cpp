
void error(const char* msg)
{
    perror(msg);
    exit(0);
}

void main(int argc, char* argv[])
{
    int sockfd, portno;
    struct sockaddr serv_addr;
    struct hostent* server;

    if(argc < 3) //kontrollerar antalet args är korrekt
    {
        fprintf(stderr, "Usage %s hostname port" argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_NET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening socket");    
    }

    server = gethostbyname(argv[1]);
    if(server == NULL)
    {
        fprintf(stderr, "Error, no such host");
        exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr_list[0], (char*)&ser_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error connecting");
    }

    printf("Please enter message: ");
    bzer(256);
    fgets(buffer, 255, stding);

    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)
    {
        error("Error writing socket");
    }

    bzero(buffer, 256);
    
    n = read(sockfd, buffer, 255);
    if(n < 0)
    {
        error("Error reading from socket");
    }

    printf("%s", buffer);
    return 0;

}