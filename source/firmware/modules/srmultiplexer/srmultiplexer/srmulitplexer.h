#include <inttypes.h>
#include "py/obj.h"

typedef struct _MultiplexChannel
{
    mp_obj_base_t base;

    size_t offset;
    size_t sizeX;
    size_t sizeY;
    size_t* values;

} MultiplexChannel;

void srmultiplexer_init(MultiplexChannel** channels, size_t* channel_count, size_t* len);