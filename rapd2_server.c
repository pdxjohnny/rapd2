#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "common.h"
#include "rapd2_ioctl.h"
#include "rapd2_comm.h"

int main(int argc, char *argv[]) {
  ssize_t msg_length = 0;
  char *tmp;
  struct rapd2_msg msg;

  if (argc > 1) {
    tmp = file_to_string(argv[1], &msg_length);
  } else {
    tmp = fd_to_string(STDIN_FILENO, &msg_length);
  }
  if (tmp == NULL) {
    printf("ERROR: You need to give some input to send to rapd2\n");
    return EXIT_FAILURE;
  }

  memcpy(msg.buffer, tmp, msg_length);
  free(tmp);
  msg.length = (__u32) msg_length;

  printf("msg.length: %d\n", msg.length);
  printf("msg.buffer: %s\n", msg.buffer);

  rapd2_comm_open();

  rapd2_comm(&msg);

  rapd2_comm_close();

  printf("msg.length: %d\n", msg.length);
  printf("msg.buffer: %s\n", msg.buffer);

  return 0;
}
