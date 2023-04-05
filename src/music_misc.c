#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <SDL_mixer.h>

typedef struct {
    const char *tag;
} Mix_MusicInterface;

struct _Mix_Music {
  Mix_MusicInterface *interface;
};

static mrb_value mrb_mix_get_music_interface_tag(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_str_new_cstr( mrb, music->interface->tag );
}

void _music_misc_init(mrb_state *mrb, struct RClass *music)
{
  mrb_define_method(mrb, music, "get_music_interface_tag", mrb_mix_get_music_interface_tag, MRB_ARGS_NONE());
}
