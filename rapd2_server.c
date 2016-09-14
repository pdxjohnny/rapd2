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

int main(int argc, char *argv[]) {
  int fd;
  ssize_t msg_length = 0;
  char ioctl_device[] = "/dev/rapd2";
  char *tmp;
  struct rapd2_msg msg;

  printf("getuid before: %d\n", getuid());

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

  fd = open(ioctl_device, O_RDWR);
  if (fd < 0) {
    perror("open /dev/rapd2: ");
    return EXIT_FAILURE;
  }

  if (ioctl(fd, RAPD2_SEND_MSG, &msg) == -1) {
    perror("when sending: ");
  }

  close(fd);

  printf("getuid after: %d\n", getuid());

  return 0;
}
