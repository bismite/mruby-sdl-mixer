
module SDL
  # Audio format flags
  AUDIO_U8 = 0x0008
  AUDIO_S8 = 0x8008
  AUDIO_U16LSB = 0x0010
  AUDIO_S16LSB = 0x8010
  AUDIO_U16MSB = 0x1010
  AUDIO_S16MSB = 0x9010
  AUDIO_U16 = AUDIO_U16LSB
  AUDIO_S16 = AUDIO_S16LSB
end

module SDL::Mixer
  # MIX_InitFlags
  MIX_INIT_FLAC   = 0x00000001
  MIX_INIT_MOD    = 0x00000002
  MIX_INIT_MP3    = 0x00000008
  MIX_INIT_OGG    = 0x00000010
  MIX_INIT_MID    = 0x00000020
  MIX_INIT_OPUS   = 0x00000040

  # Mix_Fading
  MIX_NO_FADING  = 0
  MIX_FADING_OUT = 1
  MIX_FADING_IN  = 2

  # Mix_MusicType
  MUS_NONE = 0
  MUS_CMD  = 1
  MUS_WAV  = 2
  MUS_MOD  = 3
  MUS_MID  = 4
  MUS_OGG  = 5
  MUS_MP3  = 6
  MUS_MP3_MAD_UNUSED = 7
  MUS_FLAC = 8
  MUS_MODPLUG_UNUSED = 9
  MUS_OPUS = 10

  def self.flag_to_name(flag)
    [
      :MIX_INIT_FLAC, :MIX_INIT_MOD, :MIX_INIT_MP3,
      :MIX_INIT_OGG, :MIX_INIT_MID, :MIX_INIT_OPUS
    ].select{|c|
      flag & SDL::Mixer.const_get(c) != 0
    }
  end

  def self.mucis_type_to_name(music_type)
    [
      :MUS_NONE, :MUS_CMD, :MUS_WAV, :MUS_MOD,
      :MUS_MID, :MUS_OGG, :MUS_MP3, :MUS_MP3_MAD_UNUSED,
      :MUS_FLAC, :MUS_MODPLUG_UNUSED, :MUS_OPUS
    ].each{|t|
      return t if music_type == SDL::Mixer.const_get(t)
    }
  end
end
