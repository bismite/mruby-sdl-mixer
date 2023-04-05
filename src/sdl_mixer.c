#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <SDL_mixer.h>

extern void _chunk_init(mrb_state *mrb, struct RClass *chunk);
extern void _chunk_group_init(mrb_state *mrb, struct RClass *chunk);
extern void _music_init(mrb_state *mrb, struct RClass *chunk);
extern void _music_info_init(mrb_state *mrb, struct RClass *music);
extern void _music_misc_init(mrb_state *mrb, struct RClass *music);

static mrb_value mrb_mix_init(mrb_state *mrb, mrb_value self)
{
  mrb_int flags;
  mrb_get_args(mrb, "i", &flags);
  return mrb_fixnum_value(Mix_Init(flags));
}

static mrb_value mrb_mix_open_audio(mrb_state *mrb, mrb_value self)
{
  mrb_int frequency, format, channels, chunksize;
  mrb_get_args(mrb, "iiii", &frequency, &format, &channels, &chunksize);
  return mrb_bool_value( Mix_OpenAudio(frequency, format, channels, chunksize) == 0 );
}

//
// ----
//

void mrb_mruby_sdl_mixer_gem_init(mrb_state *mrb)
{
  struct RClass *sdl = mrb_define_module(mrb, "SDL");
  struct RClass *mixer = mrb_define_module_under(mrb, sdl, "Mixer");
  struct RClass *chunk = mrb_define_class_under(mrb, mixer, "Chunk", mrb->object_class);
  MRB_SET_INSTANCE_TT(chunk, MRB_TT_DATA);
  struct RClass *music = mrb_define_class_under(mrb, mixer, "Music", mrb->object_class);
  MRB_SET_INSTANCE_TT(music, MRB_TT_DATA);
  // Initialize methods
  mrb_define_module_function(mrb, mixer, "init", mrb_mix_init, MRB_ARGS_REQ(1)); // frag
  mrb_define_module_function(mrb, mixer, "open_audio", mrb_mix_open_audio, MRB_ARGS_REQ(4)); // freq,format,channels,chunksize
  // init chunk and music
  _chunk_init(mrb,chunk);
  _chunk_group_init(mrb,chunk);
  _music_init(mrb,music);
  _music_info_init(mrb,music);
  _music_misc_init(mrb,music);
}

void mrb_mruby_sdl_mixer_gem_final(mrb_state *mrb)
{}
