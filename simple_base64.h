#ifndef __SIMPLE_BASE64_H__
#define __SIMPLE_BASE64_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/bio.h>

/**
 * Base64 encodes binary data.
 * 
 * @param input The input binary data to be encoded.
 * @param input_length The length of the input binary data.
 * @param output A pointer to the encoded output string.
 */
void base64_encode(const unsigned char *input, size_t input_length, char **output);

/**
 * Base64 decodes an encoded string back to binary data.
 * 
 * @param input The input encoded string to be decoded.
 * @param input_length The length of the input encoded string.
 * @param output A pointer to the decoded binary data.
 * @param output_length A pointer to store the length of the decoded binary data.
 */
void base64_decode(const char *input, size_t input_length, 
                    unsigned char *output, size_t *output_length);

#endif // __SIMPLE_BASE64_H__
