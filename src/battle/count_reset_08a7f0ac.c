typedef struct CountedTask {
    unsigned char active;
    char pad_01[0x97];
    unsigned int count;
    float timer;
} CountedTask;

extern void func_08A7F4BC(CountedTask *self, int id);

int func_08A7F0AC(CountedTask *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->count >= 19) {
        return -2;
    }
    func_08A7F4BC(self, 10001);
    self->timer = 0.0f;
    return 0;
}
