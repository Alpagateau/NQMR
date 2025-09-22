#include "nqme_audio.hpp"

namespace NQME
{

AudioStreamer audioStreamer;

#define FIFO_MAXMOD_UNLINKED 3

void timer3_interrupt() {
    //audioStreamer.time_ms++;
    TIMER_DATA(3)= 65503; // reload for ~1 ms
    TIMER_CR(3) |= TIMER_ENABLE; // restart timer
    //irqClear(IRQ_TIMER(3));
}

int InitAudio()
{
    audioStreamer.sys.mod_count  = 0;
    audioStreamer.sys.samp_count = 0;
    audioStreamer.sys.mem_bank   = 0;
    audioStreamer.sys.fifo_channel = FIFO_MAXMOD_UNLINKED;

    mmInit(&(audioStreamer.sys));

    audioStreamer.stream.sampling_rate	= AUDIO_SAMPLE_RATE;
	audioStreamer.stream.buffer_length	= AUDIO_BUFFER_LEN;
	audioStreamer.stream.callback		= on_stream_request;
	audioStreamer.stream.format			= MM_STREAM_8BIT_MONO;
	audioStreamer.stream.timer			= MM_TIMER0;
	audioStreamer.stream.manual			= true;

    //audioStreamer.time_ms               = 0;
    irqEnable(IRQ_TIMER(3));
    irqSet(IRQ_TIMER(3), timer3_interrupt);

    return 0;
}

mm_word on_stream_request(mm_word length, mm_addr dest, mm_stream_formats format)
{
    //REG_IME = 1;
    s8 *target = (s8*)dest;
    if(audioStreamer.audioFile == NULL)
    {
        //printf("No File\n");
        audioStreamer.isPlaying = false;
        return 0;
    }
    int a = fread((void*)target, 1, length, audioStreamer.audioFile);
    audioStreamer.samplePosition += a;
    if(feof(audioStreamer.audioFile))
    {
        //printf("goodbye\n");
        StopStream();
        return 0;
    }
    if(a == 0)
    {
        StopStream();
        return 0;
    }
    return a;
}

int PlayStream()
{
    printf("Requested\n");
    if(audioStreamer.isPlaying)
    {
        printf("Already playing\n");
        return 0;
    }
    if(audioStreamer.audioFile == NULL)
    {
        audioStreamer.isPlaying = false;
        printf("No file to play\n");
        return 1;
    }
    if(audioStreamer.samplePosition != 0)
    {
        fseek(audioStreamer.audioFile, audioStreamer.samplePosition, 0);
    }
    audioStreamer.isPlaying = true;
    printf("Starts Playing\n");
    mmStreamOpen(&(audioStreamer.stream));
    printf("Started playing\n");

    TIMER_DATA(3) = 65503;
    TIMER_CR(3) = TIMER_IRQ_REQ | TIMER_ENABLE | TIMER_DIV_1024;  // ~32.7kHz

    return 0;
}

void PauseStream()
{
    audioStreamer.isPlaying = false;
    mmStreamClose();
}

void StopStream()
{
    PauseStream();
    rewind(audioStreamer.audioFile);
    audioStreamer.samplePosition = 0;
}

int LoadSong(const char *path)
{
    audioStreamer.audioFile = fopen(path, "rb"); 
    return (audioStreamer.audioFile == NULL) ? 1 : 0;
}

void CloseStream()
{
  fclose(audioStreamer.audioFile);
  audioStreamer.samplePosition = 0;
  audioStreamer.isPlaying = false;
}
/*
int getTime()
{
    return audioStreamer.time_ms;
}
*/
}
