#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>


static int socket_fd = -1;
static void sig_handler(int s) {
	(void)s;
	syslog(LOG_INFO, "Caught signal, exiting");
	close(socket_fd);
	remove("/var/tmp/aesdsocketdata");
	exit(0);
}

int main(int argc, char** argv) {
  openlog("assignment4-server", LOG_PID, LOG_USER);

  signal(SIGINT, sig_handler);
  signal(SIGTERM, sig_handler);

  struct sockaddr_in client, server;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    syslog(LOG_INFO, "Error socket()");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(9000);
  server.sin_addr.s_addr = INADDR_ANY;

  int opt = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(socket_fd, (struct sockaddr *)&server, sizeof server) == -1) {
    syslog(LOG_INFO, "Error bind()");
    return -1;
  }

  if (listen(socket_fd, 50) == -1) {
    syslog(LOG_INFO, "Error listen()");
    return -1;
  }
  if (argc > 1 && strcmp(argv[1], "-d") == 0) {
	  if (fork() != 0) exit(0);
	  setsid();
  } 

  // receive buffer dynamic allocation
  int receive_result = 0;
  int receive_buffer_size = 4096;
  char *receive_buffer = (char *)malloc(receive_buffer_size);

  int newSocket;

  while (1) {

    int n = sizeof client;
    newSocket = accept(socket_fd, (struct sockaddr *)&client, (socklen_t *)&n);

    const char *ip = inet_ntoa(client.sin_addr);

    if (newSocket == -1) {
      syslog(LOG_ERR, "Error accept()");
    } else {
      syslog(LOG_DEBUG, "Accepted connection from %s", ip);
    }

    size_t plen = 0;
    while ((receive_result = recv(newSocket, receive_buffer + plen,
                                  receive_buffer_size - plen, 0)) > 0) {
      plen += receive_result;
      if ((int)plen + 512 > receive_buffer_size) {
        receive_buffer_size *= 2;
        receive_buffer = realloc(receive_buffer, receive_buffer_size);
      }

      char *nl = strchr(receive_buffer, '\n');
      if (nl) {
      	FILE *fptr = fopen("/var/tmp/aesdsocketdata", "a");
      	fwrite(receive_buffer, 1, plen, fptr);
      	fclose(fptr);

      	fptr = fopen("/var/tmp/aesdsocketdata", "r");
      	char sendbuffer[512];
      	int r;
      	while ((r = fread(sendbuffer, 1, sizeof sendbuffer, fptr)) > 0)
        	send(newSocket, sendbuffer, r, 0);
      	fclose(fptr);
      	break;
    	}
    }
	syslog(LOG_INFO, "Closed connection from %s", ip);
	close(newSocket);
    
  }

  close(socket_fd);

  closelog();
}
