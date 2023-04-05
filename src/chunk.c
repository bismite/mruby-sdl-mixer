#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <SDL_mixer.h>

// SDL::Mixer::Chunk
static void mrb_mix_chunk_free(mrb_state *mrb, void* ptr){ Mix_FreeChunk(ptr); }
static struct mrb_data_type const mrb_mix_chunk_data_type = { "Chunk", mrb_mix_chunk_free };

//
// ---- Chunk ----
//

static mrb_value mrb_mix_chunk_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value data;
  mrb_get_args(mrb, "S", &data );
  SDL_RWops *rw = SDL_RWFromConstMem( RSTRING_PTR(data), RSTRING_LEN(data) );
  if (NULL == rw) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "failed sound data loading.");
  }
  Mix_Chunk *chunk = Mix_LoadWAV_RW(rw, 1);
  if (chunk == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "failed sound data loading.");
  }
  mrb_data_init(self, chunk, &mrb_mix_chunk_data_type);
  return self;
}

static mrb_value mrb_mix_volume_chunk(mrb_state *mrb, mrb_value self)
{
  mrb_int volume;
  mrb_get_args(mrb, "i", &volume );
  Mix_Chunk *chunk = DATA_PTR(self);
  return mrb_fixnum_value( Mix_VolumeChunk(chunk,volume) );
}

static mrb_value mrb_mix_play_channel(mrb_state *mrb, mrb_value self)
{
  mrb_int channel, loop;
  mrb_get_args(mrb, "ii", &channel, &loop );
  Mix_Chunk *chunk = DATA_PTR(self);
  return mrb_fixnum_value( Mix_PlayChannel( channel, chunk, loop) );
}

static mrb_value mrb_mix_play_channel_timed(mrb_state *mrb, mrb_value self)
{
  mrb_int channel, loop, ticks;
  mrb_get_args(mrb, "iii", &channel, &loop, &ticks );
  Mix_Chunk *chunk = DATA_PTR(self);
  return mrb_fixnum_value( Mix_PlayChannelTimed( channel, chunk, loop, ticks) );
}

static mrb_value mrb_mix_fade_in_channel(mrb_state *mrb, mrb_value self)
{
  mrb_int channel, loop, ms;
  mrb_get_args(mrb, "iii", &channel, &loop, &ms );
  Mix_Chunk *chunk = DATA_PTR(self);
  return mrb_fixnum_value( Mix_FadeInChannel( channel, chunk, loop, ms) );
}

//
// ---- Decoder ----
//

static mrb_value mrb_mix_get_num_chunk_decoders(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(Mix_GetNumChunkDecoders());
}

static mrb_value mrb_mix_get_chunk_decoder(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_str_new_cstr(mrb, Mix_GetChunkDecoder(index));
}

static mrb_value mrb_mix_has_chunk_decoder(mrb_state *mrb, mrb_value self)
{
  mrb_value name;
  mrb_get_args(mrb, "s", &name);
  return mrb_bool_value( Mix_HasChunkDecoder( mrb_string_value_cstr(mrb,&name) ) );
}

//
// ---- Channels ----
//

static mrb_value mrb_mix_master_volume(mrb_state *mrb, mrb_value self)
{
  mrb_int vol;
  mrb_get_args(mrb, "i", &vol );
  return mrb_fixnum_value( Mix_MasterVolume(vol) );
}

static mrb_value mrb_mix_reserve_channels(mrb_state *mrb, mrb_value self)
{
  mrb_int num;
  mrb_get_args(mrb, "i", &num);
  return mrb_fixnum_value(Mix_ReserveChannels(num));
}

static mrb_value mrb_mix_allocate_channels(mrb_state *mrb, mrb_value self)
{
  mrb_int num_channels;
  mrb_get_args(mrb, "i", &num_channels);
  return mrb_fixnum_value(Mix_AllocateChannels(num_channels));
}

//
// ---- Channel ----
//

static mrb_value mrb_mix_playing(mrb_state *mrb, mrb_value self)
{
  mrb_int channel;
  mrb_get_args(mrb, "i", &channel );
  return mrb_fixnum_value( Mix_Playing(channel) );
}

static mrb_value mrb_mix_volume(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,volume;
  mrb_get_args(mrb, "ii", &channel,&volume );
  return mrb_fixnum_value( Mix_Volume(channel,volume) );
}

static mrb_value mrb_mix_set_panning(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,left,right;
  mrb_get_args(mrb, "iii", &channel,&left,&right );
  return mrb_bool_value( Mix_SetPanning(channel,left,right) != 0 );
}

static mrb_value mrb_mix_set_position(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,angle,distance;
  mrb_get_args(mrb, "iii", &channel,&angle,&distance );
  return mrb_bool_value( Mix_SetPosition(channel,angle,distance) != 0 );
}

static mrb_value mrb_mix_set_distance(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,distance;
  mrb_get_args(mrb, "ii", &channel,&distance );
  return mrb_bool_value( Mix_SetDistance(channel,distance) != 0 );
}

static mrb_value mrb_mix_set_reverse_stereo(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,flip;
  mrb_get_args(mrb, "ii", &channel,&flip );
  return mrb_bool_value( Mix_SetReverseStereo(channel,flip) != 0 );
}

//
// ---- Stop ----
//

static mrb_value mrb_mix_fade_out_channel(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,ms;
  mrb_get_args(mrb, "ii", &channel,&ms);
  return mrb_bool_value( Mix_FadeOutChannel(channel,ms) == 0 );
}

static mrb_value mrb_mix_halt_channel(mrb_state *mrb, mrb_value self)
{
  mrb_int channel;
  mrb_get_args(mrb, "i", &channel );
  return mrb_bool_value( Mix_HaltChannel(channel) == 0 );
}

static mrb_value mrb_mix_expire_channel(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,ticks;
  mrb_get_args(mrb, "ii", &channel,&ticks);
  return mrb_fixnum_value( Mix_ExpireChannel(channel,ticks) );
}

//
// ----
//

void _chunk_init(mrb_state *mrb, struct RClass *chunk)
{
  // Chunk
  mrb_define_method(mrb, chunk, "initialize", mrb_mix_chunk_initialize, MRB_ARGS_REQ(1)); // data
  mrb_define_method(mrb, chunk, "volume_chunk", mrb_mix_volume_chunk, MRB_ARGS_REQ(1)); // volume
  mrb_define_method(mrb, chunk, "play_channel", mrb_mix_play_channel, MRB_ARGS_REQ(2)); // channel, loop
  mrb_define_method(mrb, chunk, "play_channel_timed", mrb_mix_play_channel_timed, MRB_ARGS_REQ(3)); // channel, loop, ticks
  mrb_define_method(mrb, chunk, "fade_in_channel", mrb_mix_fade_in_channel, MRB_ARGS_REQ(3)); // channel, loop, ms
  // Decoders
  mrb_define_class_method(mrb, chunk, "get_num_chunk_decoders", mrb_mix_get_num_chunk_decoders, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, chunk, "get_chunk_decoder", mrb_mix_get_chunk_decoder, MRB_ARGS_REQ(1)); // index
  mrb_define_class_method(mrb, chunk, "has_chunk_decoder", mrb_mix_has_chunk_decoder, MRB_ARGS_REQ(1)); // name
  // Channels
  mrb_define_class_method(mrb, chunk, "allocate_channels", mrb_mix_allocate_channels, MRB_ARGS_REQ(1)); // num_channels
  mrb_define_class_method(mrb, chunk, "master_volume", mrb_mix_master_volume, MRB_ARGS_REQ(1)); // volume
  mrb_define_class_method(mrb, chunk, "reserve_channels", mrb_mix_reserve_channels, MRB_ARGS_REQ(1)); // num
  // Channel
  mrb_define_class_method(mrb, chunk, "playing", mrb_mix_playing, MRB_ARGS_REQ(1)); // channel
  mrb_define_class_method(mrb, chunk, "volume", mrb_mix_volume, MRB_ARGS_REQ(2)); // channel,volume
  mrb_define_class_method(mrb, chunk, "set_panning", mrb_mix_set_panning, MRB_ARGS_REQ(3)); // channel,left,right
  mrb_define_class_method(mrb, chunk, "set_position", mrb_mix_set_position, MRB_ARGS_REQ(3)); // channel,angle,distance
  mrb_define_class_method(mrb, chunk, "set_distance", mrb_mix_set_distance, MRB_ARGS_REQ(2)); // channel,distance
  mrb_define_class_method(mrb, chunk, "set_reverse_stereo", mrb_mix_set_reverse_stereo, MRB_ARGS_REQ(2)); // channel,flip
  // Stop
  mrb_define_class_method(mrb, chunk, "halt_channel", mrb_mix_halt_channel, MRB_ARGS_REQ(1)); // channel
  mrb_define_class_method(mrb, chunk, "expire_channel", mrb_mix_expire_channel, MRB_ARGS_REQ(2)); // channel,ticks
  mrb_define_class_method(mrb, chunk, "fade_out_channel", mrb_mix_fade_out_channel, MRB_ARGS_REQ(2)); // channel,ms
}
