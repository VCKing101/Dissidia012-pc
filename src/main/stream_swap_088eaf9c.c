/* Double-buffered stream: restarts the current buffer with the next track and flips. */
typedef struct StreamBuffer {
    void *stream;
    void *voice;
} StreamBuffer;

typedef struct StreamTrack {
    void *data;
    int field_04;
} StreamTrack;

typedef struct StreamPlayer {
    StreamBuffer buffers[2];
    unsigned int current;
    char pad_14[0x2C - 0x14];
    StreamTrack tracks[8];
    int count;
    int volume;
    int playing;
    int track;
} StreamPlayer;

extern void func_088A3D14(void *stream, void *voice);
extern void *func_088A3CAC(void *stream, void *data, int volume);
extern void func_088EB0A4(void);
extern void func_088A3D30(void *stream, void *voice, void (*done)(void), StreamTrack *track);
extern int func_088EB07C(StreamPlayer *player);

void func_088EAF9C(StreamPlayer *player)
{
    StreamBuffer *buffer = &player->buffers[player->current];

    if (buffer->voice != 0) {
        func_088A3D14(buffer->stream, buffer->voice);
        buffer->voice = 0;
    }
    buffer->voice = func_088A3CAC(buffer->stream, player->tracks[player->track].data, player->volume);
    func_088A3D30(buffer->stream, buffer->voice, func_088EB0A4, &player->tracks[player->track]);
    player->current++;
    if (player->current >= 2) {
        player->current = 0;
    }
    player->track = func_088EB07C(player);
}
