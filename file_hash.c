#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_hash.h"

#define BUFFER_SIZE 8192

FileHashError compute_file_sha256(char *file_path, char out_hash_str[]) {
  FILE *file = fopen(file_path, "rb");
  if (!file) {
    perror("File open error");
    return FileHashError_FileOpenError;
  }

  unsigned char buffer[BUFFER_SIZE];
  SHA256_CTX sha256_ctx;
  SHA256_Init(&sha256_ctx);

  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    SHA256_Update(&sha256_ctx, buffer, bytes_read);
  }

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_Final(hash, &sha256_ctx);

  fclose(file);

  // Two characters per byte + null terminator
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      sprintf(&out_hash_str[2 * i], "%02x", hash[i]);
  }
  out_hash_str[2 * SHA256_DIGEST_LENGTH] = '\0';
  
  return FileHashError_NoError;
}
