#include "nqmt_audio.hpp"

namespace NQMT
{

AudioStreamer audioStreamer;

#define FIFO_MAXMOD_UNLINKED 3

void timer3_interrupt() {
    audioStreamer.time_ms++;
    TIMER_DATA(3)= 65503; // reload for ~1 ms
    TIMER_CR(3) |= TIMER_ENABLE; // restart timer
    //irqClear(IRQ_TIMER(3));
}

int initAudio()
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

    audioStreamer.time_ms               = 0;
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
        audioStreamer.is_playing = false;
        return 0;
    }
    int a = fread((void*)target, 1, length, audioStreamer.audioFile);
    audioStreamer.SamplePosition += a;
    if(feof(audioStreamer.audioFile))
    {
        //printf("goodbye\n");
        stopStream();
        return 0;
    }
    if(a == 0)
    {
        stopStream();
        return 0;
    }
    return a;
}

int nqmt_playStream()
{
    printf("Requested\n");
    if(audioStreamer.is_playing)
    {
        printf("Already playing\n");
        return 0;
    }
    if(audioStreamer.audioFile == NULL)
    {
        audioStreamer.is_playing = false;
        printf("No file to play\n");
        return 1;
    }
    if(audioStreamer.SamplePosition != 0)
    {
        fseek(audioStreamer.audioFile, audioStreamer.SamplePosition, 0);
    }
    audioStreamer.is_playing = true;
    printf("Starts Playing\n");
    mmStreamOpen(&(audioStreamer.stream));
    printf("Started playing\n");

    TIMER_DATA(3) = 65503;
    TIMER_CR(3) = TIMER_IRQ_REQ | TIMER_ENABLE | TIMER_DIV_1024;  // ~32.7kHz

    return 0;
}

void pauseStream()
{
    audioStreamer.is_playing = false;
    mmStreamClose();
}

void stopStream()
{
    pauseStream();
    rewind(audioStreamer.audioFile);
    audioStreamer.SamplePosition = 0;
}

int loadSong(const char *path)
{
    audioStreamer.audioFile = fopen(path, "rb");
    //for(int i = 0; i < 20; i++){swiWaitForVBlank();
    return (audioStreamer.audioFile == NULL) ? 1 : 0;
}

int getTime()
{
    return audioStreamer.time_ms;
}

}