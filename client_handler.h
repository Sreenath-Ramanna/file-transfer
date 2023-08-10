#ifndef __CLIENT_HANDLER_H__
#define __CLIENT_HANDLER_H__

#include <pthread.h>
#include <stdint.h>

#include "file_manager.h"

typedef enum {
  stateIdle = 200,
  stateConnected = 201,
  stateBusy = 203
} ClientState;

typedef struct 
{
  int clientSocket;
  pthread_t thread;
  ClientState state;
  bool exitRequested;
  FileList *fileList;
} ClientHandlerContext;

void *clientHandlerThread(void *ctx);
void stopHandler(ClientHandlerContext *ctx);

#endif // __CLIENT_HANDLER_H__