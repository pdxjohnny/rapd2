#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
#include <linux/types.h>

#define FIRST_MINOR 1337
#define MINOR_CNT 7331

#define RAPD2_SEND_MSG _IOW(13371337, 1337, struct rapd2_msg *)
#define RAPD2_RECV_MSG _IOR(13371337, 7331, struct rapd2_msg *)

#define BUFFER_SIZE 1000

struct rapd2_msg {
  __u32 length;
  char buffer[BUFFER_SIZE];
};

#endif
