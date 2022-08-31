#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <esp_task_wdt.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "mulitplexer.h"
#include "constants.h"

TaskHandle_t loop_task_handle;

typedef struct _loop_task_args
{
    MultiplexChannel** channels;
    size_t channel_count;
    size_t counter_max;
    size_t len;
    spi_device_handle_t spi;
} loop_task_args;

static void clear_bit(uint32_t* number, size_t index)
{
    *number &= ~(1 << index);
}

void loop_task(void* pvParameters)
{
    size_t counter = 0;
    loop_task_args* args = (loop_task_args*)pvParameters;

    spi_device_handle_t spi = args->spi;
    spi_transaction_t t = { 0 };

    t.length = args->len * 8;
    t.flags = SPI_TRANS_USE_TXDATA;

    while (1)
    {   
        if (counter >= MULTIPLEXER_COUNTER_MAX)
            counter = 0;
        counter++;

        for (size_t i = 0; i < args->channel_count; i++)
        {
            MultiplexChannel* channel = args->channels[i];

            for (size_t x = 0; x < channel->sizeX; x++)
            {
                uint32_t states = UINT32_MAX; //All 1's

                for (size_t y = 0 ; y < channel->sizeY; y++)
                {
                    if (counter > channel->values[(x * channel->sizeY) + y])
                        clear_bit(&states, channel->offset + y);
                }

                //If the channel x size is 1, it is just pwm.
                //if (channel->sizeX > 1)
                    // Turn on current bit. Using common cathode rgb leds, so on = 0.
                    clear_bit(&states, channel->offset + channel->sizeY + x);

                memcpy(&t.tx_data, &states, args->len);
                spi_device_polling_transmit(spi, &t);
            }
        }
    }
}

void multiplexer_init(MultiplexChannel** channels, size_t channel_count, size_t len)
{
    esp_err_t ret;
    spi_device_handle_t spi;

    spi_bus_config_t buscfg = 
    {
        .mosi_io_num = MULTIPLEXER_DATA,
        .miso_io_num = -1,
        .sclk_io_num = MULTIPLEXER_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    spi_device_interface_config_t devcfg = 
    {
        .mode = 0,
        .clock_speed_hz = SPI_MASTER_FREQ_80M,
        .spics_io_num = MULTIPLEXER_CS,
        .flags = SPI_DEVICE_NO_DUMMY,
        .queue_size = 8,
    };

    //Initialize the SPI bus
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 0);
    ESP_ERROR_CHECK(ret);

    //Attach the Shift Registers to the SPI bus
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

    loop_task_args* args = (loop_task_args*) malloc(sizeof(loop_task_args));

    args->channels = channels;
    args->channel_count = channel_count;
    args->len = len;
    args->spi = spi;

    //Create task
    xTaskCreatePinnedToCore(
        loop_task,  
        "loop task",  
        1024 * 2,           
        (void*) args,            
        1,               
        &loop_task_handle,
        1
    );
}

MultiplexChannel* multiplexChannelFactory (size_t x, size_t y)
{
    size_t membersSize = sizeof(MultiplexChannel);
    MultiplexChannel* channel = (MultiplexChannel*) malloc(membersSize);
    
    channel->sizeX = x;
    channel->sizeY = y;
    channel->offset = 0;

    channel->values = (size_t*) malloc(sizeof(size_t) * x * y);
    return channel;
}
