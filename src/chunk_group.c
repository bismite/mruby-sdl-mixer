#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <SDL_mixer.h>

static mrb_value mrb_mix_group_channel(mrb_state *mrb, mrb_value self)
{
  mrb_int channel,tag;
  mrb_get_args(mrb, "ii", &channel,&tag);
  return mrb_bool_value( Mix_GroupChannel(channel,tag) != 0 );
}

static mrb_value mrb_mix_group_channels(mrb_state *mrb, mrb_value self)
{
  mrb_int from,to,tag;
  mrb_get_args(mrb, "iii", &from,&to,&tag);
  return mrb_bool_value( Mix_GroupChannels(from,to,tag) == 0 );
}

static mrb_value mrb_mix_group_available(mrb_state *mrb, mrb_value self)
{
  mrb_int tag;
  mrb_get_args(mrb, "i", &tag);
  return mrb_fixnum_value( Mix_GroupAvailable(tag) );
}

static mrb_value mrb_mix_group_count(mrb_state *mrb, mrb_value self)
{
  mrb_int tag;
  mrb_get_args(mrb, "i", &tag);
  return mrb_fixnum_value( Mix_GroupCount(tag) );
}

static mrb_value mrb_mix_group_oldest(mrb_state *mrb, mrb_value self)
{
  mrb_int tag;
  mrb_get_args(mrb, "i", &tag);
  return mrb_fixnum_value( Mix_GroupOldest(tag) );
}

static mrb_value mrb_mix_group_newest(mrb_state *mrb, mrb_value self)
{
  mrb_int tag;
  mrb_get_args(mrb, "i", &tag);
  return mrb_fixnum_value( Mix_GroupNewer(tag) );
}

static mrb_value mrb_mix_halt_group(mrb_state *mrb, mrb_value self)
{
  mrb_int group;
  mrb_get_args(mrb, "i", &group );
  return mrb_fixnum_value( Mix_HaltGroup(group) );
}

static mrb_value mrb_mix_fade_out_group(mrb_state *mrb, mrb_value self)
{
  mrb_int tag,ms;
  mrb_get_args(mrb, "ii", &tag,&ms);
  return mrb_fixnum_value( Mix_FadeOutGroup(tag,ms) );
}

void _chunk_group_init(mrb_state *mrb, struct RClass *chunk)
{
  mrb_define_class_method(mrb, chunk, "group_channel", mrb_mix_group_channel, MRB_ARGS_REQ(2)); // channel,tag
  mrb_define_class_method(mrb, chunk, "group_channels", mrb_mix_group_channels, MRB_ARGS_REQ(3)); // from,to,tag
  mrb_define_class_method(mrb, chunk, "group_available", mrb_mix_group_available, MRB_ARGS_REQ(3)); // tag
  mrb_define_class_method(mrb, chunk, "group_count", mrb_mix_group_count, MRB_ARGS_REQ(1)); // tag
  mrb_define_class_method(mrb, chunk, "group_oldest", mrb_mix_group_oldest, MRB_ARGS_REQ(1)); // tag
  mrb_define_class_method(mrb, chunk, "group_newest", mrb_mix_group_newest, MRB_ARGS_REQ(3)); // tag

  mrb_define_class_method(mrb, chunk, "halt_group", mrb_mix_halt_group, MRB_ARGS_REQ(1)); // tag
  mrb_define_class_method(mrb, chunk, "fade_out_group", mrb_mix_fade_out_group, MRB_ARGS_REQ(2)); // tag, ms
}
