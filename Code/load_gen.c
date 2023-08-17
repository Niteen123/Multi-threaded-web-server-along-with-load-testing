/* run using: ./load_gen localhost <server port> <number of concurrent users>
   <think time (in s)> <test duration (in s)> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
int time_up = 0;
pthread_mutex_t mutex;
FILE *log_file;

// user info struct
struct user_info
{
  // user id
  int id;

  // socket info
  int portno;
  char *hostname;
  float think_time;

  // user metrics
  int total_count;
  float total_rtt;
};

// error handling function
void error(char *msg)
{
  perror(msg);
  exit(0);
}

// time diff in seconds
float time_diff(struct timeval *t2, struct timeval *t1)
{
  return (t2->tv_sec - t1->tv_sec) + (t2->tv_usec - t1->tv_usec) / 1e6;
}
// user thread function
void *user_function(void *arg)
{
  /* get user info */
  struct user_info *info = (struct user_info *)arg;

  int sockfd, n;
  char buffer[256] = "GET /apart1 HTTP/1.0\r\nHOST: localhost\r\nContent-Type: text/plain\r\n\r\n";
  char response[2048];
  struct timeval start, end;

  struct sockaddr_in serv_addr;
  struct hostent *server;
  float totalrtt = 0.0;
  while (1)
  {
    //printf("Starting While loop again %d\n", info->id);
    pthread_mutex_lock(&mutex);
    if(time_up == 1){
      pthread_mutex_unlock(&mutex);
      break;
    }
    pthread_mutex_unlock(&mutex);
    /* start timer */
    gettimeofday(&start, NULL);

    /* TODO: create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
      error("ERROR opening socket");

    /* TODO: set server attrs */
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(info->portno);
    n = (inet_pton(AF_INET, info->hostname, &serv_addr.sin_addr));
    if (n < 0)
    {
      error("Invalid Server Address ");
      exit(1);
    }

    /* TODO: connect to server */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
      error("ERROR connecting");

    /* TODO: send message to server */
    else{
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
      error("ERROR writing to socket");

    /* TODO: read reply from server */
    //printf("User %d Line 99\n", info->id);
    n = read(sockfd, response, 2047);
    //printf("User %d Line 101\n", info->id);
    if (n < 0)
      error("ERROR reading from socket");
    /* TODO: close socket */
    close(sockfd);
    /* end timer */
    gettimeofday(&end, NULL);


    /* TODO: update user metrics */
    info->total_count++;
    info->total_rtt = time_diff(&end, &start);
    totalrtt += info->total_rtt;
    /* TODO: sleep for think time */
    usleep(info->think_time*1000000);
    }
  }

  /* exit thread */
  fprintf(log_file, "User #%d finished\n", info->id);
  fflush(log_file);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int user_count, portno, test_duration;
  float think_time;
  char *hostname;
  int n;
  if (argc != 6)
  {
    fprintf(stderr,
            "Usage: %s <hostname> <server port> <number of concurrent users> "
            "<think time (in s)> <test duration (in s)>\n",
            argv[0]);
    exit(0);
  }

  hostname = argv[1];
  portno = atoi(argv[2]);
  user_count = atoi(argv[3]);
  think_time = atof(argv[4]);
  test_duration = atoi(argv[5]);

  printf("Hostname: %s\n", hostname);
  printf("Port: %d\n", portno);
  printf("User Count: %d\n", user_count);
  printf("Think Time: %f s\n", think_time);
  printf("Test Duration: %d s\n", test_duration);

  /* open log file */
  log_file = fopen("load_gen.log", "w");

  pthread_t threads[user_count];
  struct user_info info[user_count];
  struct timeval start, end;

  /* start timer */
  gettimeofday(&start, NULL);
  int tid;
  for (int i = 0; i < user_count; ++i)
  {
    /* TODO: initialize user info */
    tid=i;
    info[i].id = tid;
    info[i].portno = portno;
    info[i].hostname = hostname;
    info[i].think_time = think_time;
    info[i].total_count = 0;
    info[i].total_rtt = 0.0;

    /* TODO: create user thread */
    n = pthread_create(&threads[i], NULL, user_function, &info[i]);
    if (n)
    {
      printf("Error:unable to create thread, %d", n);
      exit(1);
    }

    fprintf(log_file, "Created thread %d\n", i);
  }

  /* TODO: wait for test duration */
  sleep(test_duration);
  fprintf(log_file, "Woke up\n");

  /* end timer */
  time_up = 1;
  gettimeofday(&end, NULL);

  /* TODO: wait for all threads to finish*/

  // pthread_exit(NULL);
  for (int i = 0; i < user_count; i++)
  {
    if (pthread_join(threads[i], NULL) != 0)
    {
      perror("Failed to join the thread");
    }
  }

  /* TODO: print results */
  int totalrequests=0;
  float totalrtt=0.0;
  for(int i=0;i<user_count;i++)
  {
    totalrequests+= info[i].total_count;
    totalrtt += info[i].total_rtt;

  }
  fprintf(log_file, "Number of Users: %d \n", user_count);
    fprintf(log_file, "Average Throughput: %.2f \n", ((totalrequests*1.0)/test_duration));
    fprintf(log_file, "Average Response Time: %.6f \n", totalrtt/(totalrequests)*1.0);

	printf("Total Requests sent in span of %d seconds are %d and cumulative RTT is %lf seconds\n", atoi(argv[5]), totalrequests, totalrtt);
    printf("Average Throughput: %.2f \n", ((totalrequests*1.0)/test_duration));
    printf("Average Response Time: %.6f \n", totalrtt/(totalrequests)*1.0);
  /* close log file */
  fclose(log_file);

  return 0;
}
