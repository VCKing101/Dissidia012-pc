/* Restarts the map music: stops a playing track, queues the stream with its
   end callback, stops both ambient voices and the two battle cues. */
typedef struct MapMusic {
    char pad_00[4];
    int stream;
    int offset;
    char pad_0C[0x14 - 0xC];
    char player[0x38 - 0x14];
    int state;
    int voice_a;
    int voice_b;
} MapMusic;

extern void func_0889F830(void *player, int flag);
extern void func_0889F704(void *player);
extern void func_0889F6A0(void *player, int mode, int stream, int offset);
extern void func_0889F750(float volume, void *player, int flag, void (*fn)(void), MapMusic *arg);
extern void func_088967B4(void);
extern void func_088A2664(int voice, float fade);
extern void func_088A24D4(int a, int cue, int b, int c);
extern void func_08895FE4(MapMusic *self);

void func_08896240(MapMusic *self)
{
    if (self->state == 2) {
        func_0889F830(self->player, 0);
        func_0889F704(self->player);
    }
    func_0889F6A0(self->player, 3, self->stream, self->offset);
    func_0889F750(1.0f, self->player, 1, func_088967B4, self);
    self->state = 1;
    if (self->voice_a != 0) {
        func_088A2664(self->voice_a, 0.0f);
        self->voice_a = 0;
    }
    if (self->voice_b != 0) {
        func_088A2664(self->voice_b, 0.0f);
        self->voice_b = 0;
    }
    func_088A24D4(0, 0x18703, 1, 0);
    func_088A24D4(0, 0x18704, 1, 0);
    func_08895FE4(self);
}
