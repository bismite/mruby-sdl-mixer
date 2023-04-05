#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <SDL_mixer.h>

// SDL::Mixer::Music
static void mrb_mix_music_free(mrb_state *mrb, void* ptr){ Mix_FreeMusic(ptr); }
static struct mrb_data_type const mrb_mix_music_data_type = { "Music", mrb_mix_music_free };

//
// ---- Music ----
//

static mrb_value mrb_mix_music_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value data;
  mrb_get_args(mrb, "S", &data );
  SDL_RWops *rw = SDL_RWFromConstMem( RSTRING_PTR(data), RSTRING_LEN(data) );
  if (NULL == rw) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "failed music data loading.");
  }
  Mix_Music *music = Mix_LoadMUS_RW(rw, 1);
  if (music == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "failed music data loading.");
  }
  mrb_data_init(self, music, &mrb_mix_music_data_type);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@_music_data"), data); // retain
  return self;
}

static mrb_value mrb_mix_get_music_volume(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_fixnum_value( Mix_GetMusicVolume(music) );
}

static mrb_value mrb_mix_play_music(mrb_state *mrb, mrb_value self)
{
  mrb_int loop;
  mrb_get_args(mrb, "i", &loop );
  Mix_Music *music = DATA_PTR(self);
  return mrb_bool_value( Mix_PlayMusic( music, loop) == 0 );
}

static mrb_value mrb_mix_fade_in_music_pos(mrb_state *mrb, mrb_value self)
{
  mrb_int loop,ms;
  mrb_float position;
  mrb_get_args(mrb, "iif", &loop, &ms, &position );
  Mix_Music *music = DATA_PTR(self);
  return mrb_bool_value( Mix_FadeInMusicPos( music, loop, ms, position) == 0 );
}

static mrb_value mrb_mix_music_duration(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_float_value( mrb, Mix_MusicDuration(music) );
}

static mrb_value mrb_mix_get_music_position(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_float_value( mrb, Mix_GetMusicPosition(music) );
}

static mrb_value mrb_mix_get_music_type(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_fixnum_value( Mix_GetMusicType(music) );
}

//
// ---- Setting ----
//

static mrb_value mrb_mix_fading_music(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value( Mix_FadingMusic() );
}

static mrb_value mrb_mix_volume_music(mrb_state *mrb, mrb_value self)
{
  mrb_int volume;
  mrb_get_args(mrb, "i", &volume );
  return mrb_fixnum_value( Mix_VolumeMusic(volume) );
}

static mrb_value mrb_mix_playing_music(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value( Mix_PlayingMusic() != 0 );
}

static mrb_value mrb_mix_set_music_position(mrb_state *mrb, mrb_value self)
{
  mrb_float position;
  mrb_get_args(mrb, "f", &position );
  return mrb_fixnum_value( Mix_SetMusicPosition(position) == 0 );
}

//
// ---- Stop ----
//

static mrb_value mrb_mix_fade_out_music(mrb_state *mrb, mrb_value self)
{
  mrb_int ms;
  mrb_get_args(mrb, "i", &ms );
  return mrb_fixnum_value( Mix_FadeOutMusic(ms) );
}

static mrb_value mrb_mix_halt_music(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value( Mix_HaltMusic() );
}

//
// ----
//

void _music_init(mrb_state *mrb, struct RClass *music)
{
  // Music
  mrb_define_method(mrb, music, "initialize", mrb_mix_music_initialize, MRB_ARGS_REQ(1)); // data
  mrb_define_method(mrb, music, "get_music_volume", mrb_mix_get_music_volume, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "play_music", mrb_mix_play_music, MRB_ARGS_REQ(1)); // loop
  mrb_define_method(mrb, music, "fade_in_music_pos", mrb_mix_fade_in_music_pos, MRB_ARGS_REQ(3)); // loop,ms,position
  mrb_define_method(mrb, music, "music_duration", mrb_mix_music_duration, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "get_music_position", mrb_mix_get_music_position, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "get_music_type", mrb_mix_get_music_type, MRB_ARGS_NONE());
  // Music
  mrb_define_class_method(mrb, music, "volume_music", mrb_mix_volume_music, MRB_ARGS_REQ(1)); // volume(0-127)
  mrb_define_class_method(mrb, music, "fading_music", mrb_mix_fading_music, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, music, "playing_music", mrb_mix_playing_music, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, music, "set_music_position", mrb_mix_set_music_position, MRB_ARGS_REQ(1)); // position
  // Stop
  mrb_define_class_method(mrb, music, "fade_out_music", mrb_mix_fade_out_music, MRB_ARGS_REQ(1)); // ms
  mrb_define_class_method(mrb, music, "halt_music", mrb_mix_halt_music, MRB_ARGS_NONE());
}
