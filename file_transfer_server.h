#ifndef _FILE_TRANSFER_SERVER_H_
#define _FILE_TRANSFER_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef enum {
  FtError_NoError = 1000,
  FtError_ServerSocketFailed,
  FtError_ServerBindFailed,
  FtError_ServerListenFailed,
  FtError_ServerAcceptFailed,
} FtServerErrors;

/**
 * @brief Start the File Transfer server
 * 
 * @param port: Port to listen on
 * @param maxClients: max clients to support at a time
 * @return FtServerErrors 
 */
FtServerErrors start_ft_server(int port, int maxClients);
#endif // _FILE_TRANSFER_SERVER_H_
