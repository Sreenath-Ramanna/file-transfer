#include <stdio.h>
#include "file_transfer_server.h"
#include "file_manager.h"
#include "common.h"

#define FT_SERVER_FOLDER  ""

int main() {
  if(FtError_NoError != start_ft_server(FT_PORT, FT_MAX_CLIENTS)) {
    perror("Start File Transfer server failed...\n");
    return -1;
  }

  return 0;
}
