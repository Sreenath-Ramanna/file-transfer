#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "file_manager.h"

int getFileList(char *path, FileList *outFileList) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    return 1;
  }

  int fileCount = 0;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG)
    {
      fileCount++;
    }
  }
  outFileList->fileCount = fileCount;

  outFileList->list = malloc(outFileList->fileCount * sizeof(FileInfo));

  rewinddir(dir);
  fileCount = 0;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG)
    {
      outFileList->list[fileCount].fileName = strdup(entry->d_name);
      printf("hash_fn_ret: %d\n", compute_file_sha256(entry->d_name, outFileList->list[fileCount].fileHash));
      fileCount++;
    }
  }
  closedir(dir);

  return 0;
}

void cleanupFileList(FileList *fileList) {
  for(int i = 0; i < fileList->fileCount; i++) {
    if (NULL != fileList->list[i].fileName)
      free(fileList->list[i].fileName);
  }
  free(fileList->list);
}

// int main(int argc, char **argv) {
//     FileList list;
//     getFileList(argv[1], &list);

//     for(int i = 0; i < list.fileCount; i++) {
//       printf("aFile: %s, file hash: %s\n", list.list[i].fileName, list.list[i].fileHash);
//     }
    
//     cleanupFileList(&list);
    
//     return 0;
// }
