#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include "file_hash.h"

typedef struct 
{
  char *fileName;
  char fileHash[2*SHA256_DIGEST_LENGTH+1];
} FileInfo;

typedef struct 
{
  int   fileCount;
  FileInfo *list;
} FileList;

/**
 * @brief Get the File List object
 * 
 * @param path: Path at which the files to be served exist
 * @param outFileList: output, fileList structure
 * @return int: number of files available
 */
int getFileList(char *path, FileList *outFileList);

/**
 * @brief Cleanup (free) the dynamically created memory
 * 
 * @param fileList 
 */
void cleanupFileList(FileList *fileList);

#endif //__FILE_MANAGER_H__
