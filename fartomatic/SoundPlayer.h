// SOUNDPLAYER_H
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <Arduino.h>
#include <SD.h>
#include <M5Unified.h>

class SoundPlayer {
public:
  SoundPlayer();
  void begin();
  bool play(const char* filename);
private:
  static constexpr const size_t buf_num = 3;
  static constexpr const size_t buf_size = 1024;
  uint8_t wav_data[buf_num][buf_size];
  struct wav_header_t {
    char RIFF[4];
    uint32_t chunk_size;
    char WAVEfmt[8];
    uint32_t fmt_chunk_size;
    uint16_t audiofmt;
    uint16_t channel;
    uint32_t sample_rate;
    uint32_t byte_per_sec;
    uint16_t block_size;
    uint16_t bit_per_sample;
  } __attribute__((packed));

  struct sub_chunk_t {
    char identifier[4];
    uint32_t chunk_size;
    uint8_t data[1];
  } __attribute__((packed));

  bool playSdWav(const char* filename);
};

#endif  
