#ifdef ESP32S3_FDPIC_HARVARD_ALIAS
/* Replace the generic unmapper after importing the common FDPIC helpers. */
#define __dl_loadaddr_unmap __dl_loadaddr_unmap
#endif
#include "../fdpic/dl-inlines.h"

#ifdef ESP32S3_FDPIC_HARVARD_ALIAS
#include "dl-esp32s3-fdpic.h"
#endif
