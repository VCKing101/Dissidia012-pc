typedef struct RankOwner {
    char pad_000[0x14];
    int count;
    char pad_018[0x140];
    void *counter;
    void *effect;
    int field_160;
    int handle;
} RankOwner;

extern void func_08A97060(int handle, int id);
extern void func_08A67BBC(void *effect);
extern int func_08A65904(void *counter);
extern int func_08A9B8A8(RankOwner *self);

int func_08A9EE4C(RankOwner *self, void *event)
{
    if (event == 0) {
        return -1;
    }
    func_08A97060(self->handle, 0x15);
    func_08A67BBC(self->effect);
    if (func_08A65904(self->counter) == 0) {
        func_08A9B8A8(self);
    }
    return 0;
}
