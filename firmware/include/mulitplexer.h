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

MultiplexChannel* multiplexChannelFactory (size_t x, size_t y);
void multiplexer_init(MultiplexChannel** channels, size_t channel_count, size_t len);
