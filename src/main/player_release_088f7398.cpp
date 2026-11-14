/* Forwards to the sound player's virtual release() when the player exists. */
struct SoundPlayer {
    virtual void start();
    virtual void stop();
    virtual void play(int id);
    virtual void release(int id);
    virtual void pause();
    virtual void resume();
    virtual void update();
    virtual void reset();
};

extern "C" {
extern SoundPlayer *D_089D7F3C;

void func_088F7398(int id)
{
    if (D_089D7F3C != 0) {
        D_089D7F3C->release(id);
    }
}
}
