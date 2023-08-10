#include "simple_base64.h"

/**
 * Base64 encodes binary data.
 * 
 * @param input The input binary data to be encoded.
 * @param input_length The length of the input binary data.
 * @param output A pointer to the encoded output string.
 */
void base64_encode(const unsigned char *input, size_t input_length, char **output) {
  EVP_ENCODE_CTX *ctx = EVP_ENCODE_CTX_new();
  *output = (char *)malloc(EVP_ENCODE_LENGTH(input_length));
  int output_length;
  EVP_EncodeInit(ctx);
  EVP_EncodeUpdate(ctx, (unsigned char *)*output, &output_length, input, input_length);
  EVP_EncodeFinal(ctx, (unsigned char *)*output + output_length, &output_length);
  EVP_ENCODE_CTX_free(ctx);
}

/**
 * Base64 decodes an encoded string back to binary data.
 * 
 * @param input The input encoded string to be decoded.
 * @param input_length The length of the input encoded string.
 * @param output A pointer to the decoded binary data.
 * @param output_length A pointer to store the length of the decoded binary data.
 */
void base64_decode(const char *input, size_t input_length, unsigned char *output, size_t *output_length) {
    BIO *bio_mem = BIO_new_mem_buf(input, input_length);
    BIO *bio_base64 = BIO_new(BIO_f_base64());
    BIO_push(bio_base64, bio_mem);

    *output_length = BIO_read(bio_base64, output, input_length);
    BIO_free_all(bio_base64);
}
