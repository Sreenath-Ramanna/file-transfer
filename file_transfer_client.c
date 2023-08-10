#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>

#include "common.h"
#include "file_transfer_helpers.h"
#include "utils.h"
#include "file_hash.h"
#include "simple_base64.h"

#define SERVER_IP "127.0.0.1"
#define REQ_SIZE    (256)

typedef enum {
    Op_GetServerFileList = 6000, 
    Op_GetAFileFromServer,

    Op_Invalid = 6100,
} ClientOp;

ClientOp clientOp = Op_Invalid;
char *file_to_retrieve;

/**
 * @brief Print client app usage information 
 * 
 * @param argc: number of command line arguments available
 * @param argv: array of char* pointing to command line arguments
 */
void print_usage(int argc, char **argv) {
    printf("%s -l | -r remote_file\n", argv[0]);
    printf("  -l: List available files on the server\n");
    printf("  -r remote_file: Get file remote_file from server, only if not locally\n");
    printf("                  available or is different from server copy.\n\n");
    printf("  The op you can perform with the client is mutually exclusive:\n");
    printf("    * either one can list files available on server\n");
    printf("    * or one can retrieve a file from server\n");
}

/**
 * @brief Determine the Client Op intended by the user
 * 
 * @param argc: number of command line arguments available
 * @param argv: array of char* pointing to command line arguments
 */
void process_commandline_arguments(int argc, char **argv) {
    if(argc < 2) {
        print_usage(argc, argv);
        return;
    }

    // Process command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0 ) {
            if(i + 1 < argc){
                clientOp = Op_GetAFileFromServer;
                file_to_retrieve = argv[i+1];
                i++; // Skip the next argument as it's already processed
            }
            else {
                print_usage(argc, argv);
                return;
            }
        } else if (strcmp(argv[i], "-l") == 0) {
            clientOp = Op_GetServerFileList;
            return;
        }
    }
}

void send_file_list_query(int sockFd) {
    char fileListRequest[REQ_SIZE];
    size_t outSize;
    createGetFileListRequest(fileListRequest, REQ_SIZE, &outSize);
    write(sockFd, fileListRequest, outSize);
}

void parse_and_print_filelist_response(int sockFd) {
    char buffer[1024];
    // Receive and display server response
    int bytes_read = read(sockFd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
        printf("Server disconnected.\n");
    }

    buffer[bytes_read] = '\0';

    FT_FileListResponse fileListResp;
    parseFileListResponse(buffer, &fileListResp);
    printf("List of files available on server:\n");
    char *fileListCsv = getValueCopyStrForKey(buffer, FILE_NAMES_KEY);
    char **fileList = NULL;
    split_string(fileListCsv, ',', &fileList);
    for(int i = 0; i < fileListResp.fileCount; i++) {
        printf("%s\n", fileList[i]);
        free(fileList[i]);
    }
    free(fileListCsv);
}

void send_file_hash_req_and_parse_resp(int sockFd, FT_FileHashResponse *fileHashResp) {
    // create and send file hash request
    char fileHashRequest[REQ_SIZE];
    size_t outSize;
    createGetFileHashRequest(fileHashRequest, REQ_SIZE, &outSize, file_to_retrieve);
    write(sockFd, fileHashRequest, outSize);

    // parse response
    char buffer[1024];
    // Receive and display server response
    int bytes_read = read(sockFd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
        printf("Server disconnected.\n");
    }

    buffer[bytes_read] = '\0';
    // printf("hashResp: %s\n", buffer);

    parseFileHashResponse(buffer, fileHashResp);
}

void send_file_retrieval_request_and_save(int sockFd) {
    char fileDataRequest[REQ_SIZE];
    size_t outSize;
    char buffer[FILE_DATA_RESPONSE_STR_LIMIT];

    createGetFileChunkDataRequest(fileDataRequest, REQ_SIZE, 
                                &outSize, file_to_retrieve, 0);
    write(sockFd, fileDataRequest, outSize);

    int fd_being_received = open(file_to_retrieve, O_CREAT|O_WRONLY);
    size_t receivedDataChunkSize = 0;
    FT_FileChunkDataResponse response;
    do {
        receivedDataChunkSize = 0;
        bzero(buffer, FILE_DATA_RESPONSE_STR_LIMIT);

        outSize = read(sockFd, buffer, FILE_DATA_RESPONSE_STR_LIMIT);
        buffer[outSize] = '\0';
        // printf("dataResp: %s\n", buffer);
        
        parseFileChunkDataResponse(buffer, &response);
        unsigned char outData[response.chunkSize];
        printf("FileData:\n%s\n", response.fileData);
        base64_decode(response.fileData, strlen(response.fileData),
                        outData, &receivedDataChunkSize);
        printf("decoded:\n%s\n", outData);
        write(fd_being_received, outData, receivedDataChunkSize);
    } while (receivedDataChunkSize >= response.chunkSize);
    close(fd_being_received);
}

int main(int argc, char **argv) {
    int client_fd;
    struct sockaddr_in server_address;

    // determine the op
    process_commandline_arguments(argc, argv);

    // check if a valid operation is requested
    if(clientOp != Op_GetAFileFromServer && clientOp != Op_GetServerFileList){
        exit(-1);
    }

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(FT_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    if(clientOp == Op_GetServerFileList) {
        send_file_list_query(client_fd);
        parse_and_print_filelist_response(client_fd);
    }
    else if(clientOp == Op_GetAFileFromServer) {
        /**
         * @brief 
         * send fileHash query 
         * parse response it will have fileName and fileHash
         * 
         * check if the file is locally available
         * need_to_get_file_from_server = false
         * if(locally available) {
         *  local_hash = compute_sha256
         *  if (server hash == local hash) {
         *      nothing else to do
         *  }
         *  else {
         *      need_to_get_file_from_server = true
         *  }
         * }
         * else {
         *  need_to_get_file_from_server = true
         * }
         * if (need_to_get_file_from_server == true) {
         *  send get file chunk request
         *  create file locally freshly
         *  keep parsing response until a packet with data < chunkSize
         *  close file
         * }
         */
        bool need_to_get_file = false;
        char local_hash[2 * SHA256_DIGEST_LENGTH + 1];
        FileHashError error = compute_file_sha256(file_to_retrieve, local_hash);
        if (FileHashError_FileOpenError == error) {
            printf("File %s is not locally available, will be retreived from server\n", file_to_retrieve);
            need_to_get_file = true;
        }

        if(!need_to_get_file) {
            // locally file is available, confirm if hash matches with server
            FT_FileHashResponse fileHashResp;
            send_file_hash_req_and_parse_resp(client_fd, &fileHashResp);

            // printf("      name: %s\n", fileHashResp.fileName);
            // printf("      hash: %s\n", fileHashResp.fileHash);
            // printf("local_hash: %s\n", local_hash);
            if(0 != strcmp(fileHashResp.fileHash, local_hash)) {
                need_to_get_file = true;
            }
            else {
                printf("Local copy of %s matches server copy\n", file_to_retrieve);
            }
        }

        if(need_to_get_file) {
            // need to get file from server
            send_file_retrieval_request_and_save(client_fd);
        }
    }

    close(client_fd);

    return 0;
}