#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>

#include "client_handler.h"
#include "file_ops.h"
#include "file_transfer_helpers.h"
#include "common.h"
#include "simple_base64.h"

static FileInfo* get_file_info(char *req, ClientHandlerContext *context) {
  char *fileInterested = getValueCopyStrForKey(req, FILE_NAME_KEY);
  FileInfo *info = NULL;
  for(int index = 0; index < context->fileList->fileCount; index++) {
    if (0 == strncmp(context->fileList->list[index].fileName,
                      fileInterested, strlen(fileInterested))) {
      info = &context->fileList->list[index];
    }
  }
  free(fileInterested);
  return info;
}

static void send_file_data_to_client(ClientHandlerContext *context, FileInfo *info) {
  char response[FILE_DATA_RESPONSE_STR_LIMIT];
  long long fileSize = get_file_size(info->fileName);
  if(-1 != fileSize) {
    long long chunk_count = fileSize / FILE_DEFAULT_CHUNK_SIZE;
    if(0 != (fileSize % FILE_DEFAULT_CHUNK_SIZE)) {
      chunk_count++;
    }

    int fd = open(info->fileName, O_RDONLY);
    for(int i = 0; i < chunk_count; i++){
      char data[FILE_DEFAULT_CHUNK_SIZE];

      int bytes_read = read(fd, (void*)data, FILE_DEFAULT_CHUNK_SIZE);
      char *file_data_base64;
      base64_encode((unsigned char*) data, bytes_read, &file_data_base64);
      sprintf(response, FILE_DATA_RESPONSE_PATTERN, info->fileName, FILE_DEFAULT_CHUNK_SIZE, i, "TODO", file_data_base64);
      write(context->clientSocket, response, strlen(response));
      fsync(context->clientSocket);
    }
  }
}

static void process_client_request(char *request, ClientHandlerContext *context) {
  char response[RESPONSE_STR_LIMIT];

  context->state = stateBusy;

  bzero((void*) response, RESPONSE_STR_LIMIT);
  
  printf("Inside process_client_request\n");

  if (NULL != strstr(request, OP_GET_FILE_LIST)) {
    sprintf(response, FILE_LIST_RESPONSE_PATTERN, context->fileList->fileCount, 0);

    for(int index = 0; index < context->fileList->fileCount; index++) {
      if (strlen(response) + strlen(context->fileList->list[index].fileName) >= RESPONSE_STR_LIMIT) {
        // send the response so far built and reinitialize a new response
        write(context->clientSocket, response, strlen(response));
        bzero((void*) response, RESPONSE_STR_LIMIT);
        sprintf(response, FILE_LIST_RESPONSE_PATTERN, context->fileList->fileCount, index);
      }
      strcat(response, context->fileList->list[index].fileName);
      if(index < context->fileList->fileCount - 1)
        strcat(response, ",");
    }

    if (strlen(response) != 0) {
      strcat(response, ";\n");
      write(context->clientSocket, response, strlen(response));
    } 
    else
      write(context->clientSocket, ";\n", 2);
  }
  else if(NULL != strstr(request, OP_GET_FILE_HASH)) {
    FileInfo *info = get_file_info(request, context);
    char *fileInterested = getValueCopyStrForKey(request, FILE_NAME_KEY);
    if (NULL != info) {
      sprintf(response, FILE_HASH_RESPONSE_PATTERN, info->fileName, info->fileHash);
      write(context->clientSocket, response, strlen(response));
    }
    else {
      printf("ERROR: OP_GET_FILE_HASH, file not found error: %s\n", fileInterested);
    }
    free(fileInterested);
  }
  else if(NULL != strstr(request, OP_GET_FILE_DATA_CHUNK)) {
    //FILE_DATA_RESPONSE_PATTERN
    FileInfo *info = get_file_info(request, context);
    char *fileInterested = getValueCopyStrForKey(request, FILE_NAME_KEY);
    if (NULL != info) {
      send_file_data_to_client(context, info);
    }
    else {
      printf("ERROR: OP_GET_FILE_HASH, file not found error: %s\n", fileInterested);
    }
    free(fileInterested);
  }

  context->state = stateConnected;
}

void* clientHandlerThread(void *ctx) {
  ClientHandlerContext *context = (ClientHandlerContext*) ctx;
  context->state = stateConnected;
  char buffer[1024];
  bool server_exiting = false;


  while (!server_exiting) {
    printf("Client connected. Inside client handler thread.\n");

    // Receive data from client
    int bytes_read = read(context->clientSocket, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
      printf("Client disconnected.\n");
      break;
    }

    // Null-terminate the received data
    buffer[bytes_read] = '\0';

    printf("Received from client: %s", buffer);
    process_client_request(buffer, context);

    // Check if the received line is "quit\n"
    if (strcmp(buffer, "quit\n") == 0) {
      printf("Closing connection.\n");
      break;
    }
    else if (strcmp(buffer, SERVER_EXIT_PROMPT) == 0) {
      server_exiting = true;
      write(context->clientSocket, "quit\n", strlen("quit\n"));
      break;
    }
  }

  close(context->clientSocket);
  context->state = stateIdle;
  printf("Client socket closed.\n");
  return NULL;
}

void stopHandler(ClientHandlerContext *ctx) {
  ctx->exitRequested = true;
}
