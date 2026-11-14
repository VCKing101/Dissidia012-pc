typedef struct SoundCtrl {
    char pad_000[0x148];
    char lock[0x10];
    int sound;
    int effect;
} SoundCtrl;

extern int func_08A7E904(void *lock, int key);
extern void func_08A7E858(void *lock, int key);
extern int func_08A65904(int sound);
extern void func_08A6941C(int effect);
extern void func_08A9B8A8(SoundCtrl *self);

/* Event: restarts the effect when unlocked or while the sound plays, finishing when it is silent; -1 bad event. */
int func_08A9F540(SoundCtrl *self, void *event)
{
    int key;
    if (event == 0) {
        return -1;
    }
    key = 1;
    if (func_08A7E904(self->lock, key) == 0 || func_08A65904(self->sound) == 1) {
        func_08A6941C(self->effect);
        if (func_08A65904(self->sound) == 0) {
            func_08A9B8A8(self);
        }
    }
    func_08A7E858(self->lock, key);
    return 0;
}
