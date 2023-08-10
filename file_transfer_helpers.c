#include "file_transfer_helpers.h"
#include "utils.h"
#include <sys/stat.h>

// request creator functions
RequestError createGetFileListRequest(char *outGetFileListReq, 
                              size_t maxOutBufLen,
                              size_t *outRequestLength) {
  *outRequestLength = snprintf(outGetFileListReq, maxOutBufLen, "%s;\n", OP_GET_FILE_LIST);
  if(-1 == *outRequestLength) {
    return RequestError_OutputBufferTooSmall;
  }

  return RequestError_NoError;
}

RequestError createGetFileHashRequest(char *outGetFileHashReq, 
                              size_t maxOutBufLen,
                              size_t *outRequestLength, 
                              char *fileName) {
  if (NULL == fileName || 0 == strlen(fileName)) {
    return RequestError_MissingFileName;
  }

  *outRequestLength = snprintf(outGetFileHashReq, 
                              maxOutBufLen, 
                              "%s;%s:%s;\n", OP_GET_FILE_HASH, FILE_NAME_KEY, fileName);
  if(-1 == *outRequestLength) {
    return RequestError_OutputBufferTooSmall;
  }
  return RequestError_NoError;
}

RequestError createGetFileChunksHashesRequest(char *outGetFileChunksHashesRequestReq, 
                              size_t maxOutBufLen,
                              size_t *outRequestLength, 
                              char *fileName) {
  if (NULL == fileName || 0 == strlen(fileName)) {
    return RequestError_MissingFileName;
  }

  *outRequestLength = snprintf(outGetFileChunksHashesRequestReq, 
                              maxOutBufLen, 
                              "%s;%s:%s;\n", OP_GET_FILE_CHUNKS_HASH, FILE_NAME_KEY, fileName);
  if(-1 == *outRequestLength) {
    return RequestError_OutputBufferTooSmall;
  }
  return RequestError_NoError;
}

RequestError createGetFileChunkDataRequest(char * outFileChunkDataRequest,
                              size_t maxOutBufLen,
                              size_t *outRequestLength, 
                              char *fileName,
                              size_t chunkIndex) {
  if (NULL == fileName || 0 == strlen(fileName)) {
    return RequestError_MissingFileName;
  }

  *outRequestLength = snprintf(outFileChunkDataRequest, 
                              maxOutBufLen, 
                              "%s;%s:%s;%s:%ld;\n", OP_GET_FILE_DATA_CHUNK, 
                                    FILE_NAME_KEY, fileName,
                                    CHUNK_INDEX_KEY, chunkIndex);
  if(-1 == *outRequestLength) {
    return RequestError_OutputBufferTooSmall;
  }

  return RequestError_NoError;
}

// response parser functions
char *getValueCopyStrForKey(char *rawResp, char *key) {
  char *valueStart = strstr(rawResp, key) + strlen(key) + 1; // to skip ":"
  // if(0 == strcmp(key, FILE_HASH_KEY)) {
  //   printf("=> %s %s\n", rawResp, valueStart);
  // }
  char *valueEnd = strstr(valueStart, ";");
  char *valueCopy = strndup(valueStart, valueEnd - valueStart);
  return valueCopy;
}

ResponseError parseFileListResponse(char *rawResponse, FT_FileListResponse *outParsedResponse) {
  outParsedResponse->rawResponse = rawResponse;
  char *fileCountCopy = getValueCopyStrForKey(rawResponse, FILE_COUNT_KEY);
  outParsedResponse->fileCount = atol(fileCountCopy);
  free(fileCountCopy);

  return ResponseError_NoError;
}

ResponseError parseFileHashResponse(char *rawResponse, FT_FileHashResponse *outParsedResponse){
  outParsedResponse->rawResponse = rawResponse;
  outParsedResponse->fileName = getValueCopyStrForKey(rawResponse, FILE_NAME_KEY);
  outParsedResponse->fileHash = getValueCopyStrForKey(rawResponse, FILE_HASH_KEY);
}

void cleanupFileHashResponse(FT_FileHashResponse *parsedResponse) {
  free(parsedResponse->fileName);
  free(parsedResponse->fileHash);
}

ResponseError parseFileChunksHashesResponse(char *rawResponse, 
                                            FT_FileChunksHashesResponse *outParsedResponse) {
  outParsedResponse->rawResponse = rawResponse;
  return ResponseError_NoError;
}

ResponseError parseFileChunkDataResponse(char *rawResponse,
                                          FT_FileChunkDataResponse *outParsedResponse) {
  outParsedResponse->rawResponse = rawResponse;
  outParsedResponse->fileName = getValueCopyStrForKey(rawResponse, FILE_NAME_KEY);
  
  char *chunkSizeCopy = getValueCopyStrForKey(rawResponse, CHUNK_SIZE_KEY);
  outParsedResponse->chunkSize = atol(chunkSizeCopy);
  free(chunkSizeCopy);

  char *chunkIndexCopy = getValueCopyStrForKey(rawResponse, CHUNK_INDEX_KEY);
  outParsedResponse->chunkIndex = atol(chunkIndexCopy);
  free(chunkIndexCopy);

  outParsedResponse->chunkHash = getValueCopyStrForKey(rawResponse, CHUNK_HASH_KEY);
  outParsedResponse->fileData = getValueCopyStrForKey(rawResponse, FILE_DATA_KEY);
  return ResponseError_NoError;
}

void cleanupFileChunkDataResponse(FT_FileChunkDataResponse *parsedResponse) {
  free(parsedResponse->fileName);
  free(parsedResponse->chunkHash);
  free(parsedResponse->fileData);
}

char *getNthStringInValue(char *csvList, int n) {
  char **values;
  char *result = NULL;
  int countValues = split_string(csvList, ',', &values);

  if(n >= countValues) {
    return result;
  }
  result = strdup(values[n]);

  for(int i = 0; i < countValues; i++) {
    free(values[i]);
  }
  free(values);
  return result;
}


long long get_file_size(char *path) {
    struct stat file_stat;

    if (stat(path, &file_stat) == 0) {
        return (long long) file_stat.st_size;
    } else {
        perror("Error getting file size");
    }

    return -1;
}
