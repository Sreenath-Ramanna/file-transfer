#include "sim.h"

int main() {
  const unsigned char input_data[] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
  size_t input_length = sizeof(input_data);
  char *encoded_output = NULL;
  unsigned char decoded_output[input_length];
  size_t decoded_length = input_length;

  // Encode
  base64_encode(input_data, input_length, &encoded_output);
  printf("Base64 Encoded: %s\n", encoded_output);

  // Decode
  base64_decode(encoded_output, strlen(encoded_output), (unsigned char*)decoded_output, &decoded_length);
  printf("Decode len: %ld, Base64 Decoded: ", decoded_length);
  for (size_t i = 0; i < decoded_length; i++) {
      printf("%02X ", decoded_output[i]);
  }
  printf("\n");

  free(encoded_output);
  // free(decoded_output);

  return 0;
}
