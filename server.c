// Created by Pratik Kanani and Amrita Dhir

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

void servicePlayers(int socket_des1, int socket_des2)
{
    int toto_total_score = 0, titi_total_score = 0;
    int network_content = 0;
    char message[100];

    fprintf(stderr, "Starting game...\n");
    while (1)
    {
        // for toto
        strcpy(message, "You can now play");
        write(socket_des1, message, strlen(message) + 1);
        if (!read(socket_des1, &network_content, 4))
        {
            close(socket_des1);
            close(socket_des2);
            fprintf(stderr, "Error\n");
            exit(1);
        }
        toto_total_score += network_content;
        fprintf(stderr, "TOTO : %d and  total score: %d\n", network_content, toto_total_score);

        // for titi
        strcpy(message, "You can now play");
        write(socket_des2, message, strlen(message) + 1);
        if (!read(socket_des2, &network_content, 4))
        {
            close(socket_des1);
            close(socket_des2);
            fprintf(stderr, "Error\n");
            exit(1);
        }
        titi_total_score += network_content;
        fprintf(stderr, "TITI: %d and total score: %d\n\n", network_content, titi_total_score);

        if (toto_total_score >= 100)
        {
            strcpy(message, "Game over: you won the game");
            write(socket_des1, message, strlen(message) + 1);
            strcpy(message, "Game over: you lost the game");
            write(socket_des2, message, strlen(message) + 1);
            fprintf(stderr, "TOTO won the game\n");
            close(socket_des1);
            close(socket_des2);
            exit(0);
        }
        else if (titi_total_score >= 100)
        {
            strcpy(message, "Game over: you won the game");
            write(socket_des2, message, strlen(message) + 1);
            strcpy(message, "Game over: you lost the game");
            write(socket_des1, message, strlen(message) + 1);
            fprintf(stderr, "TITI won the game\n");
            close(socket_des1);
            close(socket_des2);
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servAdd;
    int serversocket, p1, p2, port;

    if (argc != 2)
    {
        printf("Call model: %s <Port #>\n", argv[0]);
        exit(0);
    }
    if ((serversocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Error ! socket is not created\n");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf(argv[1], "%d", &port);
    servAdd.sin_port = htons((uint16_t)port);
    bind(serversocket, (struct sockaddr *)&servAdd, sizeof(servAdd));
    listen(serversocket, 10);
    while (1)
    {
        printf("Waiting for players........\n");
        p1 = accept(serversocket, (struct sockaddr *)NULL, NULL);
        printf("Found TOTO\n");
        p2 = accept(serversocket, (struct sockaddr *)NULL, NULL);
        printf("Found TITI\n");
        if (!fork())
            servicePlayers(p1, p2);
        close(p1);
        close(p2);
    }
}
