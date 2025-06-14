#ifndef NQMT_AUDIO_H 
#define NQMT_AUDIO_H

#include <nds.h>
#include <stdio.h>
#include <maxmod9.h>

#include "nqmt_fs.hpp"

#define AUDIO_BUFFER_LEN 1216
#define AUDIO_SAMPLE_RATE 22050

/*This header is responsible for all audio, from streaming to simple sounds*/

typedef struct {
    FILE* audioFile;
    long int SamplePosition;
    int sample_rate;
    mm_ds_system sys;
    mm_stream stream;
    bool is_playing;
    long int time_ms;
}AudioStreamer;

int initAudio();

mm_word on_stream_request(mm_word length, mm_addr dest, mm_stream_formats format);

int  loadSong(const char *path);
int  nqmt_playStream();
void nqmt_pauseStream();
void nqmt_stopStream();

int getTime();

#endif