/* Releases the playing stream through the sound player. */
struct SoundPlayer {
    virtual void start();
    virtual void stop();
    virtual void play(int id);
    virtual void release(int id);
};

struct VoiceState {
    int field_00;
    int voice;
    int stream;
};

extern "C" {
extern VoiceState D_09B69B00;
extern SoundPlayer *D_089D7F3C;

void func_088F7108(void)
{
    VoiceState *state = &D_09B69B00;

    if (state->stream != 0) {
        D_089D7F3C->release(state->stream);
        state->stream = 0;
    }
}
}
