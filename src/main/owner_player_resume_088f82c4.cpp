/* Calls virtual resume() of the track object at 0x14C70. */
struct PlayerTrack {
    virtual void start();
    virtual void stop();
    virtual void play();
    virtual void release();
    virtual void pause();
    virtual void resume();
    virtual void update();
    virtual void reset();
};

struct PlayerOwner {
    char pad_00000[0x14C70];
    PlayerTrack *player;
};

extern "C" {
void func_088F82C4(PlayerOwner *owner)
{
    owner->player->resume();
}
}
