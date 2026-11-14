typedef struct RankOwner {
    char pad_000[0x158];
    void *counter;
    void *effect;
} RankOwner;

extern int func_08A69564(void *effect);
extern int func_08A65904(void *counter);
extern int func_08A9B734(RankOwner *self);
extern int func_08A9B8A8(RankOwner *self);

int func_08A9F840(RankOwner *self, void *event)
{
    if (event == 0) {
        return -1;
    }
    func_08A69564(self->effect);
    if (func_08A65904(self->counter) == 0) {
        func_08A9B734(self);
        func_08A9B8A8(self);
    }
    return 0;
}
