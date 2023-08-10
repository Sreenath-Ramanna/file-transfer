#ifndef __FILE_TRANSFER_OPS_H__
#define __FILE_TRANSFER_OPS_H__

#define OP_GET_FILE_LIST        "GetFilesList"
#define OP_GET_FILE_HASH        "GetFileHash"
#define OP_GET_FILE_CHUNKS_HASH "GetFileChunksHashes"
#define OP_GET_FILE_DATA_CHUNK  "GetFileDataChunk"
#define OP_DISCONNECT           "Disconnect"

#define OP_FILE_LIST_RESPONSE        "FilesListResp"
#define OP_FILE_HASH_RESPONSE        "FilesHashResp"
#define OP_FILE_CHUNKS_HASH_RESPONSE "FilesChunksHashesResp"
#define OP_FILE_DATA_CHUNK_RESPONSE  "FileDataChunkResp"

#define FILE_NAME_KEY             "FileNameKey"
#define FILE_NAMES_KEY            "FileNamesKey"
#define FILE_COUNT_KEY            "TotalFileCountKey"
#define FILE_START_INDEX_KEY      "FilesStartIndexKey"
#define FILE_HASH_KEY             "FileHashKey"
#define CHUNK_SIZE_KEY            "ChunkSizeKey"
#define CHUNK_COUNT_KEY           "TotalChunksKey"
#define CHUNK_START_INDEX_KEY     "ChunkStartIndexKey"
#define CHUNK_INDEX_KEY           "ChunkIndexKey"
#define CHUNK_HASH_KEY            "ChunkHashKey"
#define FILE_DATA_KEY             "FileDataKey"

/**
 * @brief file request response patterns
 * Get files list request: 
 *  "GetFilesList\n"
 * Get files response: 
 *  "FilesList; TotalFileCount: N; FilesStartIndex:1, FileNames: fileName_1, fileName_2, ..., fileName_i\n"
 *  "FilesList; TotalFileCount: N; FilesStartIndex:i+1, FileNames: fileName_i+1, fileName_i+2, ..., fileName_2i\n" ...
 * 
 * Get file hash request: 
 *  "GetFileHash; FileName: fileName\n"
 * Get file hash response: 
 *  "FileHash; FileName: fileName; FileHash: file_SHA-256_as_string\n"
 * 
 * Get file chunks hashses request: 
 *  "GetFileChunksHashes; FileName: fileName\n"
 * Get file chunks hashses response: 
 *  "FileChunksHashes; FileName: fileName; ChunkSize: S; TotalChunks: N; ChunkStartIndex: 1, Hashes: Chunk_1_SHA-1, Chunk_2_SHA-1, ... Chunk_i_SHA-1\n"
 *  "FileChunksHashes; FileName: fileName; ChunkSize: S; TotalChunks: N; ChunkStartIndex: i+1, Hashes: Chunk_i+1_SHA-1, Chunk_i+2_SHA-1, ... Chunk_2i_SHA-1\n"
 * 
 * Get file data chunk request:
 *  "GetFileDataChunk; FileName: fileName, ChunkIndex: i"
 *  "FileDataChunk; FileName: fileName; ChunkSize: S; ChunkIndex: i; ChunkHash: chunk_SHA-1; FileData: bas64_encoded_file_data"
 */

#define FILE_LIST_RESPONSE_PATTERN "FilesListResp;TotalFileCountKey:%d;FilesStartIndexKey:%d,FileNamesKey:"
#define FILE_HASH_RESPONSE_PATTERN "FileHashResp;FileNameKey:%s;FileHashKey:%s;\n"
#define FILE_DATA_RESPONSE_PATTERN "FileDataChunkResp;FileNameKey:%s;ChunkSizeKey:%d;ChunkIndexKey:%d;ChunkHashKey:%s;FileDataKey:%s\n"
#endif // __FILE_TRANSFER_OPS_H__
