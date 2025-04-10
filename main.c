#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
//#include "socket.h"

#define DEFAULT_PORT "3002"
#define DEFAULT_HOST "z690-fc41"
#define DEFAULT_CLOG "./cpptest.clog"
#define DEFAULT_OVERWRITE 1

void print_syntax()
{

}

const char *getArg(int argc, char **argv, const char *key, const char *default_value)
{
  for ( int i = 1; i < argc; ++i )
  {
    if ( 0 == strncmp(key, (const char *)argv[i], strlen(key) > strlen(argv[i]) ? strlen(argv[i]) : strlen(key)) )
    {
      return argv[i+1];
    }
  }

  return default_value;
}

int getPresence(int argc, char **argv, const char *key, int default_value)
{
  for ( int i = 1; i < argc; ++i )
  {
    if ( 0 == strncmp(key, (const char *)argv[i], strlen(key) > strlen(argv[i]) ? strlen(argv[i]) : strlen(key)) )
    {
      return 1;
    }
  }

  return default_value;
}

int main(int argc, char **argv)
{
  int port = atoi(getArg(argc, argv, "-port", DEFAULT_PORT));
  const char *clog = getArg(argc, argv, "-clog", DEFAULT_CLOG);
  int overwrite_clog = getPresence(argc, argv, "-overwrite", DEFAULT_OVERWRITE);


  printf("Port: %d\nCoverage Log: %s\nOverwrite Coverage Log: %s\n\n", port, clog, overwrite_clog ? "Yes" : "No");

  printf("Waiting for instrumented code to phone home...\n");

  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);

  int hServer = socket(AF_INET, SOCK_STREAM, 0);

  if ( -1 == hServer )
  {
    printf("Unable to create a socket, bailing...\n");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  int ret = 0;
  if ( ret = bind(hServer, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
  {
    printf("Unable to bind socket: Return: %d, Errno: %d\n", ret, errno);
    return 1;
  }

  if ( ret = listen(hServer, 1) < 0 )
  {
    printf("Unable to listen on socket. Return: %d, Errno: %d\n", ret, errno);
    return 1;
  }

  int hClient = accept(hServer, (struct sockaddr *)&client_addr, &client_len);

  if ( hClient < 0 )
  {
    printf("Unable to accept connection to client. Errno: %d\n", errno);
    return 1;
  }

  char client_ip[INET_ADDRSTRLEN];

  if ( inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN) == NULL)
  {
    printf("Couldn't ascertain the client IP address. Skipping...\n");
  }
  else
  {
    printf("Connected to instrumented binary phoning from: %s\n", client_ip);
  }
  
  size_t size = 0xFFFF;
  unsigned char buffer[size];

  FILE *hClog = fopen(clog, overwrite_clog ? "w" : "a");

  if ( 0 == hClog )
  {
    printf("Unable to open/create clog file: %s, errno: %d\n", clog, errno);
  }
  else
  {
    printf("Reading coverage data...\n");
    
    ssize_t readCount = 0, totalReadCount = 0, totalWriteCount = 0;
    
    while ((readCount = read(hClient, &buffer, size)) > 0)
    {
      int writeCount = fwrite(&buffer, sizeof(unsigned char), readCount, hClog);
      totalWriteCount += writeCount;

      if ( readCount != writeCount )
      {
        printf("Read/Write byte mismatch. Errno: %d, Read %d bytes, Wrote %d bytes\n", errno, readCount, writeCount);
        break;
      }

      totalReadCount += readCount;
    }

    printf("Read Success: %d bytes\n", totalReadCount);

    fclose(hClog);

    printf("Coverage log file written: %d bytes\nLocation: %s\n", totalWriteCount, clog);
  }

  close(hClient);
  close(hServer);

  return 0;
}