typedef struct RankOwner {
    char pad_000[0x158];
    void *counter;
    void *effect;
} RankOwner;

extern void func_08A6AC44(void *effect);

int func_08A9F674(RankOwner *self, void *event)
{
    if (event == 0) {
        return -1;
    }
    func_08A6AC44(self->effect);
    return 0;
}
