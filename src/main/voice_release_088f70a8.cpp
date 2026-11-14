/* Releases the playing voice through the sound player. */
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

void func_088F70A8(void)
{
    VoiceState *state = &D_09B69B00;

    if (state->voice != -1) {
        D_089D7F3C->play(state->voice);
        state->voice = -1;
    }
}
}
