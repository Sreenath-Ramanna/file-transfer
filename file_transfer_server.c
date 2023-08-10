//  #include <signal.h> - if we want to install a Ctrl+c handler
#include <stdbool.h>
#include "file_transfer_server.h"
#include "client_handler.h"
#include "file_manager.h"
#include "common.h"

#define WAIT_TIME_100ms     (100 * 1000)

FileList fileListToServe;

static ClientHandlerContext *next_free_handler(ClientHandlerContext *handlers, int count) {
    ClientHandlerContext *result = NULL;
    for(int i = 0; i < count; i++) {
        if (handlers[i].state == stateIdle) {
            result = &handlers[i];
        }
    }
    return result;
}

FtServerErrors start_ft_server(int port, int maxClients) {
    int server_fd, new_socket, client_count = 0;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    int fileCount = getFileList(SERVER_FOLDER, &fileListToServe);

    ClientHandlerContext client_context[maxClients];
    for(int i = 0; i < maxClients; i++) {
        client_context[i].state = stateIdle;
        client_context[i].fileList = &fileListToServe;
    }
    ClientHandlerContext *free_handler_ctx = NULL;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    bool server_exiting = false;

    while (!server_exiting) {
        // check if any client handler has completed its work
        // while all client handlers are busy do a wait-sleep 100ms
        do {
            usleep(WAIT_TIME_100ms);
            free_handler_ctx = next_free_handler(&client_context[0], maxClients);
        }while (free_handler_ctx == NULL);

        printf("A free client context available, server listening...\n");
        
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // create client handler thread and save its context
        free_handler_ctx->clientSocket = new_socket;
        pthread_create(&free_handler_ctx->thread, NULL, 
                        clientHandlerThread, (void*)free_handler_ctx);
    }

    close(server_fd);

    return 0;
}
