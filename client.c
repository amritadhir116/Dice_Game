// Created by Pratik Kanani and Amrita Dhir

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char message[100];
    int server, port,total=0;
    struct sockaddr_in servAdd;

    if (argc != 3)
    {
        printf("Call model:%s <IP> <Port#>\n", argv[0]);
        exit(0);
    }
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    sscanf(argv[2], "%d", &port);
    servAdd.sin_port = htons((uint16_t)port);
    if (inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0)
    {
        fprintf(stderr, " inet_pton() has failed\n");
        exit(2);
    }
    if (connect(server, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
    {
        fprintf(stderr, "connect() has failed, exiting\n");
        exit(3);
    }
    while (1)
    {
        if (read(server, message, 100) < 0)
        {
            fprintf(stderr, "read() error\n");
            exit(3);
        }
        fprintf(stderr, "From Server: %s\n", message);

        if (!strcmp(message, "You can now play"))
        {
            sleep(1);
            int dice = (int)time(0) % 10 + 1;
            total +=dice;
            printf("Dice score: %d and Total score : %d \n\n", dice,total);
            write(server, &dice, 4);
        }
        else if (!strcmp(message, "Game over: you won the game"))
        {
            printf("I won the game\n");
            exit(0);
        }
        else if (!strcmp(message, "Game over: you lost the game"))
        {
            printf("I lost the game\n");
            exit(0);
        }
    }
}