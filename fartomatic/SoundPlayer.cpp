//soundplayer.cpp
#include "SoundPlayer.h"
#include <esp_log.h>

SoundPlayer::SoundPlayer() {}

void SoundPlayer::begin() {
    SD.begin();
}

bool SoundPlayer::play(const char* filename) {
    return playSdWav(filename);
}

bool SoundPlayer::playSdWav(const char* filename) {
    auto file = SD.open(filename);

    if (!file) {
        return false;
    }

    wav_header_t wav_header;
    file.read((uint8_t*)&wav_header, sizeof(wav_header_t));

    if (memcmp(wav_header.RIFF, "RIFF", 4)
        || memcmp(wav_header.WAVEfmt, "WAVEfmt ", 8)
        || wav_header.audiofmt != 1
        || wav_header.bit_per_sample < 8
        || wav_header.bit_per_sample > 16
        || wav_header.channel == 0
        || wav_header.channel > 2) {
        file.close();
        return false;
    }

    file.seek(offsetof(wav_header_t, audiofmt) + wav_header.fmt_chunk_size);
    sub_chunk_t sub_chunk;

    file.read((uint8_t*)&sub_chunk, 8);

    while (memcmp(sub_chunk.identifier, "data", 4)) {
        if (!file.seek(sub_chunk.chunk_size, SeekMode::SeekCur)) {
            break;
        }
        file.read((uint8_t*)&sub_chunk, 8);
    }

    if (memcmp(sub_chunk.identifier, "data", 4)) {
        file.close();
        return false;
    }

    int32_t data_len = sub_chunk.chunk_size;
    bool flg_16bit = (wav_header.bit_per_sample >> 4);

    size_t idx = 0;
    while (data_len > 0) {
        size_t len = data_len < buf_size ? data_len : buf_size;
        len = file.read(wav_data[idx], len);
        data_len -= len;

        if (flg_16bit) {
            M5.Speaker.playRaw((const int16_t*)wav_data[idx], len >> 1, wav_header.sample_rate, wav_header.channel > 1, 1, 0);
        } else {
            M5.Speaker.playRaw((const uint8_t*)wav_data[idx], len, wav_header.sample_rate, wav_header.channel > 1, 1, 0);
        }
        idx = idx < (buf_num - 1) ? idx + 1 : 0;
    }
    file.close();

    return true;
}
