#ifndef __FILE_TRANSFER_HELPERS_H__
#define __FILE_TRANSFER_HELPERS_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "file_ops.h"

typedef enum {
  RequestError_NoError = 2000,
  RequestError_MissingFileName,
  RequestError_OutputBufferTooSmall,
} RequestError;


typedef enum {
  ResponseError_NoError = 3000,
  ResponseError_RemoteFileNotFound,
  ResponseError_InvalidChunkIndex,
} ResponseError;

typedef struct {
  char      *rawResponse;
  uint32_t  fileCount;
} FT_FileListResponse;

typedef struct {
  char  *rawResponse;
  char  *fileName;
  char  *fileHash;
} FT_FileHashResponse;

typedef struct {
  char *rawResponse;
  char *fileName;
  uint32_t  chunkSize;
  uint32_t  chunksCount;
  uint32_t  chunksStartIndex;
} FT_FileChunksHashesResponse;

typedef struct 
{
  char *rawResponse;
  char *fileName;
  uint32_t chunkSize;
  uint32_t chunkIndex;
  char *chunkHash;
  char *fileData;
} FT_FileChunkDataResponse;

/**
 * @brief Create a Get File List Request object
 * Create a request to retrieve available file list from the server
 * Sample request: "GetFilesList\n"
 * 
 * @param outGetFileListReq: output, constructed request 
 * @param maxOutBufLen: input, available space in output buffer
 * @param *outRequestLength: output, actual space used by the constructed request
 * @return RequestError: error
 */
RequestError createGetFileListRequest(char *outGetFileListReq, 
                              size_t maxOutBufLen,
                              size_t *outRequestLength);


/**
 * @brief Create a Get File Hash Request object
 * Sample Request: "GetFileHash;FileName:fileName\n"
 * 
 * @param outGetFileHashReq: output, constructed request 
 * @param maxOutBufLen: input, available space in output buffer
 * @param *outRequestLength: output, actual space used by the constructed request
 * @param fileName: file for which hash is to be retrived from server
 * @return RequestError: error
 */
RequestError createGetFileHashRequest(char *outGetFileHashReq, 
                              size_t maxOutBufLen,
                              size_t *outRequestLength, 
                              char *fileName);


/**
 * @brief Create a Get File Chunks Hashes Request object
 * Sample request: "GetFileChunksHashes;FileName:fileName\n"
 * 
 * @param outGetFileChunksHashesRequestReq: output, constructed request 
 * @param maxOutBufLen: input, available space in output buffer
 * @param *outRequestLength: output, actual space used by the constructed request
 * @param fileName: file for which hashes are to be retrived from server
 * @return RequestError: error
 */
RequestError createGetFileChunksHashesRequest(char *outGetFileChunksHashesRequestReq, 
                              size_t maxOutBufLen,
                              size_t *outRequestLength, 
                              char *fileName);

/**
 * @brief Create a Get File Chunk Data Request object
 * Sample request: "GetFileDataChunk;FileName:fileName;ChunkIndex:i"
 * 
 * @param outFileChunkDataRequest: output, constructed request 
 * @param maxOutBufLen: input, available space in output buffer
 * @param *outRequestLength: output, actual space used by the constructed request
 * @param fileName: file for which hash is to be retrived
 * @param chunkIndex: index of the file chunk of which data is to be retrieved from server 
 * @return RequestError: error
 */
RequestError createGetFileChunkDataRequest(char * outFileChunkDataRequest,
                              size_t maxOutBufLen,
                              size_t *outRequestLength, 
                              char *fileName,
                              size_t chunkIndex);

/**
 * @brief Parse response and populate FileListResponse object
 * 
 * @param rawResponse: input, file list response from server
 * @param outParsedResponse: output, parsed FT_FileListResponse object
 * @return ResponseError: error
 */
ResponseError parseFileListResponse(char *rawResponse, FT_FileListResponse *outParsedResponse);

/**
 * @brief Parse response and populate FileHashResponse object
 * 
 * @param rawResponse: input, file hash response from server
 * @param outParsedResponse: output, parsed FT_FileHashResponse object
 * @return ResponseError: error
 */
ResponseError parseFileHashResponse(char *rawResponse, FT_FileHashResponse *outParsedResponse);

/**
 * @brief Parse response and populate FileChunksHashesResponse object
 * 
 * @param rawResponse: input, file chunks hashes response from server
 * @param outParsedResponse: output, parsed FT_FileChunksHashesResponse
 * @return ResponseError: error
 */
ResponseError parseFileChunksHashesResponse(char *rawResponse, 
                                            FT_FileChunksHashesResponse *outParsedResponse);
/**
 * @brief Parse response and populate FileChunkDataResponse object
 * 
 * @param rawResponse: input, file chunk data response from server
 * @param outParsedResponse: output, parsed FT_FileChunkDataResponse
 * @return ResponseError: error
 */
ResponseError parseFileChunkDataResponse(char *rawResponse,
                                          FT_FileChunkDataResponse *outParsedResponse);

/**
 * @brief cleanup (free) dynamically allocate memory
 * 
 * @param parsedResponse: previously parsed FT_FileHashResponse data
 */
void cleanupFileHashResponse(FT_FileHashResponse *parsedResponse);

/**
 * @brief cleanup (free) dynamically allocate memory
 * 
 * @param parsedResponse: previously parsed FT_FileChunkDataResponse data
 */
void cleanupFileChunkDataResponse(FT_FileChunkDataResponse *parsedResponse);

/**
 * @brief Get the Nth String In Value object
 * 
 * @param csvList 
 * @param n 
 * @return char* a dynamically allocated string (needs freeing by the caller)
 */
char *getNthStringInValue(char *csvList, int n);

/**
 * @brief Get the Value Copy Str For Key object
 * 
 * @param rawRespReq: A request or a response string
 * @param key: Key for which value is to be extracted
 * @return char* a dynamically allocated copy of value as string
 */
char *getValueCopyStrForKey(char *rawResp, char *key);

/**
 * @brief Get the file size object
 * 
 * @param path 
 * @return long long 
 */
long long get_file_size(char *path);
#endif //__FILE_TRANSFER_HELPERS_H__
