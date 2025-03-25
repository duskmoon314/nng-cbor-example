#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <nng/nng.h>
#include <nng/protocol/pipeline0/push.h>

#include <cbor.h>
#include <cbor/data.h>
#include <cbor/maps.h>
#include <cbor/serialization.h>
#include <cbor/strings.h>

int main(int argc, char **argv) {
  nng_socket sock;
  int rv;

  // Create an IPC PUSH socket
  if ((rv = nng_push0_open(&sock)) != 0) {
    fprintf(stderr, "nng_push0_open: %s\n", nng_strerror(rv));
    exit(1);
  }

  // Dial on the socket
  if ((rv = nng_dial(sock, "ipc:///tmp/nng_cbor_example.ipc", NULL, 0)) !=
      0) {
    fprintf(stderr, "nng_dial: %s\n", nng_strerror(rv));
    exit(1);
  }

  printf("Producer is ready, dialling on ipc:///tmp/nng_cbor_example.ipc\n");

  // Send 10 messages
  for (int i = 0; i < 10; ++i) {
    // Prepare the message
    // JSON format: {"key": i, "value": "Hello from C!"}
    cbor_item_t *item = cbor_new_definite_map(2);

    bool success = cbor_map_add(
        item, (struct cbor_pair){.key = cbor_move(cbor_build_string("key")),
                                 .value = cbor_move(cbor_build_uint8(i))});

    success &= cbor_map_add(
        item, (struct cbor_pair){
                  .key = cbor_move(cbor_build_string("value")),
                  .value = cbor_move(cbor_build_string("Hello from C!"))});

    if (!success) {
      fprintf(stderr, "Failed to create CBOR item\n");
      exit(1);
    }

    // Serialize the message
    unsigned char *buffer;
    size_t buffer_size;

    cbor_serialize_alloc(item, &buffer, &buffer_size);

    printf("Sending message %d\n", i);

    // Send the message
    if ((rv = nng_send(sock, buffer, buffer_size, 0)) != 0) {
      // if (rv != NNG_EAGAIN) {
      fprintf(stderr, "nng_send: %d %s\n", rv, nng_strerror(rv));
      exit(1);
      // }
    }

    // Free the CBOR item and buffer
    cbor_decref(&item);
    free(buffer);
  }

  // Close the socket
  nng_close(sock);

  printf("Example finished\n");
  return 0;
}