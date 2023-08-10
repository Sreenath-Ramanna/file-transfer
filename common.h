#ifndef _COMMON_H__
#define _COMMON_H__

#define FT_MAX_CLIENTS    1
#define FT_PORT           4340

#define SERVER_EXIT_PROMPT    "quit_n_quit_server\n"

#define SERVER_FOLDER "."
#define CLIENT_FOLDER "."


#define WAIT_TIME_10MS                10000

#define RESPONSE_STR_LIMIT            1024
#define FILE_DEFAULT_CHUNK_SIZE       4096
#define FILE_DATA_RESPONSE_STR_LIMIT  (FILE_DEFAULT_CHUNK_SIZE + RESPONSE_STR_LIMIT)

#endif //_COMMON_H__