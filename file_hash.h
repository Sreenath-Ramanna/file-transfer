#ifndef __FILE_HASH_H__
#define __FILE_HASH_H__

#include <openssl/sha.h>

typedef enum {
  FileHashError_NoError = 5000,
  FileHashError_FileOpenError,
  FileHashError_OutputBufferTooSmall,
} FileHashError;

/**
 * @brief Compute SHA-256 hash for a given file
 * 
 * @param file_path: input, file path
 * @param out_hash_str: output, buffer to hold file's SHA-256, must be at least 
 *                  2 * SHA256_DIGEST_LENGTH + 1 byes long.
 *                  The SHA-256 is returned as a printable string, not a sequence
 *                  of hex bytes.
 * @return FileHashError: error
 */
FileHashError compute_file_sha256(char *file_path, char *out_hash_str);
#endif //__FILE_HASH_H__