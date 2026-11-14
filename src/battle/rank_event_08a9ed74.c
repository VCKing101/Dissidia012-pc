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
extern void func_08A67B78(void *effect);
extern int func_08A65904(void *counter);
extern int func_08A9B8A8(RankOwner *self);

int func_08A9ED74(RankOwner *self, void *event)
{
    if (event == 0) {
        return -1;
    }
    func_08A97060(self->handle, 13);
    func_08A67B78(self->effect);
    if (func_08A65904(self->counter) == 0) {
        func_08A9B8A8(self);
    }
    self->count++;
    return 0;
}
