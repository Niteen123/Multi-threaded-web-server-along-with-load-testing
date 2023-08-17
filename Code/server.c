#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include "process.h"
#define size 4096
#define threadcount 4
int serverSocket;
pthread_t threadIds[threadcount];
#define queuesize 30000

int shared_buffer[queuesize];

int requests = 0;
int ConnectionCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueueFull, condQueueempty;

// The Thread Runner Function
void *thread_handler(void *argv)
{
    int threadID = *((int *)argv);
    threadID = threadID + 1;
    while (1)
    {

        pthread_mutex_lock(&mutexQueue);
        while (requests == 0)
        {
            pthread_cond_wait(&condQueueempty, &mutexQueue);
        }
        int client = -1;
        for (int i = 0; i < queuesize; i++)
        {
            if (shared_buffer[i] != -1)
            {
                client = shared_buffer[i];
                shared_buffer[i] = -1;
                break;
            }
        }
        requests--;
        pthread_cond_signal(&condQueueFull);
        pthread_mutex_unlock(&mutexQueue);

        char buffer[size];
        char *dataToBeSent;
        bzero(buffer, size);

        int n = read(client, (void *)buffer, size);

        dataToBeSent = processRequest(buffer);

        write(client, dataToBeSent, strnlen(dataToBeSent, size));
        free(dataToBeSent);
        close(client);
    }
}
void handler(int sig)
{
    for(int i=0;i<threadcount;i++)
    {
        pthread_kill(threadIds[i],SIGKILL);
    }
    for(int i=0;i<threadcount;i++)
    {
        pthread_join(threadIds[i],NULL);
    }
}
int main(int argc, char const *argv[])
{
    signal(SIGINT,handler);
    int sockfd, portno;
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Create server-listener socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("ERROR opening socket");

    // Set server address
    struct sockaddr_in serv_addr;
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind server to specific port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    // Spin up listener
    if (listen(sockfd, queuesize) < 0)
    {
        printf("Error in listening\n");
        exit(1);
    }

    for (int i = 0; i < queuesize; i++)
    {
        shared_buffer[i] = -1;
    }
    int id;
    for (int i = 0; i < threadcount; i++)
    {
        pthread_t threadId;
        id = i;
        if (pthread_create(&threadId, NULL, thread_handler, &id) != 0)
        {
            perror("pthread_create");
        }
        threadIds[i] = threadId;
    }
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    socklen_t clientSocketLength = sizeof(cli_addr);
    while (1)
    {
    pthread_mutex_lock(&mutexQueue);
        while (requests == queuesize)
        {
            pthread_cond_wait(&condQueueFull, &mutexQueue);
        }
        pthread_mutex_unlock(&mutexQueue);
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            printf("Error in accept\n");
            continue;
        }
        pthread_mutex_lock(&mutexQueue);

        for (int i = 0; i < queuesize; i++)
        {
            if (shared_buffer[i] == -1)
            {
                shared_buffer[i] = newsockfd;
                requests++;
                break;
            }
        }

        // conditional fill signal so worker threads can take it up.
        pthread_cond_signal(&condQueueempty);
        pthread_mutex_unlock(&mutexQueue);
    }
}
