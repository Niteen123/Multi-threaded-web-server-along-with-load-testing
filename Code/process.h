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
#include "http_server.h"
#include <signal.h>
#include <time.h>

#define size 4096

char *readFileData(char PATH[]);
char *createresponse(int statusCode, char *httpVersion, char data[size]);
char *getstring(struct HTTP_Response response);
void tokenise(struct HTTP_Request *request,char data[size])
{
    char delim[] = "\n";
    int word = 0;


    char *ptr = strtok(data, delim);
    while (ptr != NULL && word < 3)
    {
        char delim2[] = " ";
        char *headers = strtok(ptr, delim2);
        while (headers != NULL)
        {

            if (word == 0)
                strcpy(request->method, headers);

            if (word == 1)
                strcpy(request->url, headers);

            word += 1;
            headers = strtok(NULL, delim2);
        }
        ptr = strtok(NULL, delim);
    }
}
char *processRequest(char data[size])
{
    struct HTTP_Request request;
    tokenise(&request,data);

    if (strcmp(request.method, "GET") == 0)
    {
        struct stat stats;
        char PATH[size];
        strcpy(PATH, "./html_files");
        char indexfile[] = "/index.html";
        strncat(PATH, request.url, strnlen(request.url, size));
        if (stat(PATH, &stats) == 0)
        {
            if (S_ISDIR(stats.st_mode))
            {
                strncat(PATH, indexfile, strlen(indexfile));
            }
            
            char *fileData = readFileData(PATH);
            strcpy(data, fileData);
            free(fileData);
            return createresponse(200, request.HTTP_version, data);
        }
        else
        {
            strcpy(data, "Path Not Found.");
            return createresponse(404, request.HTTP_version, data);
        }
    }
    else
    {
        strcpy(data, "Unsupported Method Request!");
        return createresponse(500, request.HTTP_version, data);
    }
}

char *createresponse(int statusCode, char *httpVersion, char data[size])
{
    struct HTTP_Response response;
    strcpy(response.HTTP_version, httpVersion);
    sprintf(response.content_length, "%d", (int)strnlen(data, size));
    strcpy(response.content_type, "text/html");
    strcpy(response.body, data);
    switch (statusCode)
    {
    
    case 404:
        strcpy(response.status_code, "404");
        strcpy(response.status_text, "Not Found");
        break;
    case 200:

        strcpy(response.status_code, "200");
        strcpy(response.status_text, "OK");
        break;
    default:
        strcpy(response.status_code, "500");
        strcpy(response.status_text, "Unsupported Method ");
        break;
    }

    return getstring(response);
}

char *getstring(struct HTTP_Response response)
{
    char *data = (char *)malloc(size * sizeof(char));
    bzero(data, size);

    strcpy(data, "HTTP/1.1");
    strcat(data, " ");

    strcat(data, response.status_code);
    strcat(data, " ");

    strcat(data, response.status_text);
    strcat(data, " ");
    strcat(data, "\r\n");

    strcat(data, "Content-Type: ");
    strcat(data, response.content_type);
    strcat(data, "\r\n");

    strcat(data, "Content-Length: ");
    strcat(data, response.content_length);
    strcat(data, "\r\n\n");

    strcat(data, response.body);

    return data;
}

// The utility function for file read.
char *readFileData(char PATH[])
{
    FILE *ptr;
    char *data = (char *)malloc(size * sizeof(char));
    ptr = fopen(PATH, "r");
    char var[50];

    if (NULL == ptr)
    {
        printf("file can't be opened \n");
    }

    while (fgets(var, 50, ptr) != NULL)
    {
        strncat(data, var, 50);
    }

    fclose(ptr);
    return data;
}
