#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "rapd2_ioctl.h"
#include "rapd2_comm.h"

static int rapd2_fd = -1;

int rapd2_comm_open() {
  rapd2_fd = open(RAPD2_IOCTL_DEVICE, O_RDWR);
  if (rapd2_fd < 0) {
    perror("open " RAPD2_IOCTL_DEVICE ": ");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int rapd2_comm_close() {
  return close(rapd2_fd);
}

int rapd2_comm(struct rapd2_msg * msg) {
  if (rapd2_fd < 0) {
    perror("connection to driver not open: ");
    return EXIT_FAILURE;
  }

  if (ioctl(rapd2_fd, RAPD2_SEND_MSG, msg) == -1) {
    perror("when sending to rapd2 module: ");
    return EXIT_FAILURE;
  }

  /*
  if (ioctl(rapd2_fd, RAPD2_RECV_MSG, &msg) == -1) {
    perror("when recving from rapd2 module: ");
    return EXIT_FAILURE;
  }
  */

  return EXIT_SUCCESS;
}
