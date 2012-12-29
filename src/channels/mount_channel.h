/*
 * mount_channel.h
 *
 *  Created on: Dec 6, 2011
 *      Author: dazo
 */

#ifndef MOUNT_CHANNEL_H_
#define MOUNT_CHANNEL_H_

#include <openssl/sha.h> /* SHA_DIGEST_LENGTH, SHA_CTX */
#include <zmq.h>
#include "src/loader/sel_ldr.h"
#include "src/main/etag.h"
#include "api/zvm.h"

EXTERN_C_BEGIN

#define MAX_CHANNELS_NUMBER 6548

/* name, id, access type, gets, getsize, puts, putsize */
#define CHANNEL_ATTRIBUTES ChannelAttributesNumber

/* stdin, stdout, stderr. in the future 2 more channels will be added */
#define RESERVED_CHANNELS 3
#define NET_BUFFER_SIZE 0x10000

/* reserved zerovm channels names */
#define STDIN "/dev/stdin" /* c90 stdin */
#define STDOUT "/dev/stdout" /* c90 stdout */
#define STDERR "/dev/stderr" /* c90 stderr */
#define STDDBG "/dev/debug" /* zvm extension */

/* not used so far */
#define INPUT "/dev/input" /* random access read-only channel */
#define OUTPUT "/dev/input" /* random access write-only channel */

/* attributes has fixed order, thats why enum has been used */
enum ChannelAttributes {
  ChannelName,
  ChannelAlias,
  ChannelAccessType,
  ChannelGets,
  ChannelGetSize,
  ChannelPuts,
  ChannelPutSize,
  ChannelAttributesNumber
};

/* source file types */
enum ChannelSourceType {
  ChannelRegular, /* supported */
  ChannelDirectory, /* not supported */
  ChannelCharacter, /* supported. under construction */
  ChannelBlock, /* not tested */
  ChannelFIFO, /* not tested */
  ChannelLink, /* not tested */
  ChannelSocket, /* not tested (ChannelIPC replacement) */
  ChannelIPC, /* to remove */
  ChannelTCP, /* supported */
  ChannelINPROC, /* not supported */
  ChannelPGM, /* not supported */
  ChannelEPGM, /* not supported */
  ChannelUDP, /* going to be supported in the future */
  ChannelSourceTypeNumber
};

/* source file prefixes */
#define CHANNEL_SOURCE_PREFIXES { \
  "file", /* ChannelRegular */\
  "directory", /* ChannelDirectory */\
  "character", /*ChannelCharacter*/\
  "block", /* ChannelBlock */\
  "fifo", /* ChannelFIFO */\
  "link", /* ChannelLink */ \
  "socket", /* ChannelSocket */\
  "ipc", /* ChannelIPC */\
  "tcp", /* ChannelTCP */\
  "inproc", /* ChannelINPROC */\
  "pgm", /* ChannelPGM */\
  "epgm", /* ChannelEPGM */\
  "udp", /* ChannelUDP */\
  "invalid"\
}

/*
 * zerovm channel descriptor. part of information available for the user side
 * todo(d'b): replace "closed" with "flags" field. will contain EOF, errors, e.t.c.
 */
struct ChannelDesc
{
  char *name; /* real name */
  char *alias; /* name for untrusted */
  void *tag; /* tag context */
  char digest[TAG_DIGEST_SIZE]; /* tag hexadecimal digest */

  int32_t handle; /* file handle. fit only for regular files. ### replace it with socket? */
  void *socket; /* can be used both by network and local channel */

  /* group #2.1 */
  int64_t size; /* channel size */
  /* group #2.2 */
  zmq_msg_t msg; /* 0mq message container. should be initialized */
  int32_t bufpos; /* index of the 1st available byte in the buffer */
  int32_t bufend; /* index of the 1st unavailable byte in the buffer */

  enum AccessType type; /* type of access sequential/random */
  enum ChannelSourceType source; /* network or local file */
  int64_t getpos; /* read position */
  int64_t putpos; /* write position */

  /* limits and counters */
  int64_t limits[IOLimitsCount];
  int64_t counters[IOLimitsCount];

  /* added to serve sequential channels */
  int8_t eof; /* if not 0 the channel reached eof at the last operation */
};

/* construct all channels, initialize it and update system_manifest */
void ChannelsCtor(struct NaClApp *nap);

/* close all channels, initialize it and update system_manifest */
void ChannelsDtor(struct NaClApp *nap);

/* get string contain protocol name by channel source type */
char *StringizeChannelSourceType(enum ChannelSourceType type);

EXTERN_C_END

#endif /* MOUNT_CHANNEL_H_ */