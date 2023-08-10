#ifndef __ENV_CONFIG__
#define __ENV_CONFIG__

#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#define FT_DEFAULT_PORT 4343
#define PORT_ENV_NAME   "FT_PORT"

#define FT_DEFAULT_MAX_CLIENTS    10
#define MAX_CLIENTS_ENV_NAME      "MAX_FT_CLIENTS"

uint16_t getServerPort() {
  char *portFromEnv = getenv(PORT_ENV_NAME);
  if (NULL == portFromEnv) {
    return FT_DEFAULT_PORT;
  }

  uint16_t portValue = strtoul(portFromEnv, NULL, 10);
  if (errno == EINVAL || errno == ERANGE) {
    return FT_DEFAULT_PORT;
  }
  return portValue;
}

uint16_t getMaxClients() {
  char *maxClientsFromEnv = getenv(MAX_CLIENTS_ENV_NAME);
  if (NULL == maxClientsFromEnv) {
    return FT_DEFAULT_MAX_CLIENTS;
  }

  uint16_t maxClientsValue = strtoul(maxClientsFromEnv, NULL, 10);
  if (errno == EINVAL || errno == ERANGE) {
    return FT_DEFAULT_MAX_CLIENTS;
  }
  return maxClientsValue;
}

#endif // __ENV_CONFIG__
