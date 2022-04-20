#pragma once

#include <inttypes.h>

typedef struct MultiplexChannel MultiplexChannel;

struct MultiplexChannel
{
    size_t offset;
    size_t sizeX;
    size_t sizeY;
    size_t* values;
};

MultiplexChannel* multiplexChannelFactory (size_t x, size_t y)
{
    size_t membersSize = sizeof(size_t) * 3 + sizeof(void*);
    MultiplexChannel* channel = (MultiplexChannel*) malloc(membersSize);
    channel->values = (size_t*) malloc(sizeof(size_t) * x * y);
    return channel;
}

void multiplexer_init(MultiplexChannel** channels, size_t* channel_count, size_t* len);
