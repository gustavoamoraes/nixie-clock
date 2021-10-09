#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "srmulitplexer.h"

const mp_obj_type_t multiplexchannel_type;

STATIC mp_obj_t multiplexchannel_make_new(const mp_obj_type_t* type, size_t n_args, size_t n_kw, const mp_obj_t* args)
{
    mp_arg_check_num(n_args, n_kw, 2, 2, true);
    MultiplexChannel* self = m_new_obj(MultiplexChannel);
    self->base.type = &multiplexchannel_type;
    self->sizeX = mp_obj_get_int(args[0]);
    self->sizeY = mp_obj_get_int(args[1]);
    self->values = malloc(sizeof(size_t) * self->sizeX * self->sizeY);

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t set_channel_values(mp_obj_t self_in, mp_obj_t values)
{
    MultiplexChannel* self = MP_OBJ_TO_PTR(self_in);

    mp_obj_iter_buf_t iter_buf;
    mp_obj_t item, iterable = mp_getiter(values, &iter_buf);

    int i = 0;

    while ((item = mp_iternext(iterable)) != MP_OBJ_STOP_ITERATION)
    {
        uint8_t value = mp_obj_get_int(item);
        self->values[i] = value;

        i++;
    }

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(set_channel_values_obj, set_channel_values);

STATIC mp_obj_t start_multiplexer (mp_obj_t _channels)
{
    mp_obj_iter_buf_t iter_buf;
    mp_obj_t item, iterable = mp_getiter(_channels, &iter_buf);
       
    size_t* channel_count = malloc(sizeof(size_t));
    size_t* sr_count = malloc(sizeof(size_t)); 

    *channel_count = mp_obj_get_int( mp_obj_len_maybe(_channels) );

    MultiplexChannel** channels = malloc(*channel_count * sizeof(MultiplexChannel*));

    size_t offset = 0;
    size_t i = 0;

    while ((item = mp_iternext(iterable)) != MP_OBJ_STOP_ITERATION)
    {
        channels[i] = MP_OBJ_TO_PTR(item);
        channels[i]->offset = offset;

        //If the channel x size is 1 don't need to add to the offset, it's just pwm and the y pins are enough for 1.
        offset += channels[i]->sizeY + (channels[i]->sizeX == 1? 0: channels[i]->sizeX);
        i++;
    }

    //Shift registors needed.
    *sr_count = ceil(offset / 8.0); 

    srmultiplexer_init(channels, channel_count, sr_count);

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(start_multiplexer_obj, start_multiplexer);

//Register class

STATIC const mp_rom_map_elem_t multiplexc_locals_dict_table[] =
{
    { MP_ROM_QSTR(MP_QSTR_set_values), MP_ROM_PTR(&set_channel_values_obj) },
};

STATIC MP_DEFINE_CONST_DICT(multiplexc_locals_dict, multiplexc_locals_dict_table);

const mp_obj_type_t multiplexchannel_type =
{
    { &mp_type_type },
    .name = MP_QSTR_multiplex_channel,
    .make_new = multiplexchannel_make_new,
    .locals_dict = (mp_obj_dict_t*)&multiplexc_locals_dict,
};

//Register module

STATIC const mp_rom_map_elem_t srmultiplexer_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_srmultiplexer) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_multiplexchannel), (mp_obj_t)&multiplexchannel_type },
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&start_multiplexer_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_srmultiplexer_globals, srmultiplexer_globals_table);

// Define module object.
const mp_obj_module_t srmultiplexer_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_srmultiplexer_globals,
};

MP_REGISTER_MODULE(MP_QSTR_srmultiplexer, srmultiplexer_user_cmodule, 1);
