#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <SDL_mixer.h>

static mrb_value mrb_mix_get_music_title(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_str_new_cstr( mrb, Mix_GetMusicTitle(music) );
}

static mrb_value mrb_mix_get_music_title_tag(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_str_new_cstr( mrb, Mix_GetMusicTitleTag(music) );
}

static mrb_value mrb_mix_get_music_artist_tag(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_str_new_cstr( mrb, Mix_GetMusicArtistTag(music) );
}

static mrb_value mrb_mix_get_music_album_tag(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_str_new_cstr( mrb, Mix_GetMusicAlbumTag(music) );
}

static mrb_value mrb_mix_get_music_copyright_tag(mrb_state *mrb, mrb_value self)
{
  Mix_Music *music = DATA_PTR(self);
  return mrb_str_new_cstr( mrb, Mix_GetMusicCopyrightTag(music) );
}

void _music_info_init(mrb_state *mrb, struct RClass *music)
{
  mrb_define_method(mrb, music, "get_music_title", mrb_mix_get_music_title, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "get_music_title_tag", mrb_mix_get_music_title_tag, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "get_music_artist_tag", mrb_mix_get_music_artist_tag, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "get_music_album_tag", mrb_mix_get_music_album_tag, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "get_music_copyright_tag", mrb_mix_get_music_copyright_tag, MRB_ARGS_NONE());
}
