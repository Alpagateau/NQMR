#ifndef NQME_AUDIO_H 
#define NQME_AUDIO_H

#include <nds.h>
#include <stdio.h>
#include <maxmod9.h>

#include "nqme_fs.hpp"

#define AUDIO_BUFFER_LEN 1216
#define AUDIO_SAMPLE_RATE 11025 //22050

/*This header is responsible for all audio, from streaming to simple sounds*/

namespace NQME{

typedef struct {
    FILE* audioFile;
    long int samplePosition;
    int sampleRate;
    mm_ds_system sys;
    mm_stream stream;
    bool isPlaying;
    //long int time_ms;
}AudioStreamer;

int InitAudio();

mm_word on_stream_request(mm_word length, mm_addr dest, mm_stream_formats format);

int  LoadSong(const char *path);
int  PlayStream();
void PauseStream();
void StopStream();
void CloseStream();

}
#endif
