#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <syslog.h>
#include <errno.h>
#include <sys/stat.h>
//globals
int server_sock = -1;
int client_sock = -1;
char *data_file = "/var/tmp/aesdsocketdata";
volatile sig_atomic_t stop = 0;
//signal handler
void sig_handler(int signo) {
    syslog(LOG_INFO, "Caught signal, exiting");
    stop = 1; // stop listnening flag
}

int main(int argc, char *argv[])
{
    //daemon initialliaztion
    int daemon_mode = 0;

    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
      daemon_mode = 1;
    }
    //signal handler initiallization
   struct sigaction sa;
   sa.sa_handler = sig_handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sigaction(SIGINT, &sa, NULL);
   sigaction(SIGTERM, &sa, NULL);
   //socket initiallization
   struct sockaddr_in server_addr, client_addr;
   socklen_t client_len = sizeof(client_addr);

   server_sock = socket(AF_INET, SOCK_STREAM, 0);
   if (server_sock < 0) {
     perror("socket");
     exit(EXIT_FAILURE);
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(9000);
   int optval = 1;
   setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

   if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
     perror("bind");
     close(server_sock);
     exit(EXIT_FAILURE);
   }
   //daemon start
   if (daemon_mode) {
      pid_t pid = fork();

      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
      }

      if (pid > 0) {
        exit(EXIT_SUCCESS); // father exit
      }

    // son is the daemon
      umask(0);
      setsid();
      chdir("/");

      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);
   } 
   //start listening
   if (listen(server_sock, 10) < 0) {
     perror("listen");
     close(server_sock);
     exit(EXIT_FAILURE);
   }
   openlog("aesdsocket", LOG_PID, LOG_USER);

   while (!stop) {
     client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
     if (client_sock < 0) {
        if (errno == EINTR && stop) break; 
           perror("accept");
        continue;
    }

     char client_ip[INET_ADDRSTRLEN];
     inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
     syslog(LOG_INFO, "Accepted connection from %s", client_ip);

    // recieve data and handle newline
     char buffer[1024];
     FILE *fp = fopen(data_file, "a+");
     if (!fp) {
        perror("fopen");
        close(client_sock);
        continue;
     }

     ssize_t n;
     while ((n = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {

      
        fwrite(buffer, 1, n, fp);

        if (strchr(buffer, '\n')) {
            fflush(fp);
	    break;
        
        }
       
     }   
     fclose(fp);

    //resend to client
     fp = fopen(data_file, "r");
     if (fp) {
        while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
            send(client_sock, buffer, n, 0);
        }
        fclose(fp);
     }

     syslog(LOG_INFO, "Closed connection from %s", client_ip);
     close(client_sock);
}

closelog();
close(server_sock);
remove(data_file); 

 }
