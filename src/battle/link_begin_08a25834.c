typedef struct LinkBegin {
    unsigned char active;
    char pad_01[0x3];
    int state;
    float elapsed;
    int field_0C;
    int field_10;
    char field_14[0x4];
} LinkBegin;

extern void func_089F84E0(void *sub);
extern void func_089F85B0(void *sub, int value, int result);

int func_08A25834(LinkBegin *self, int value, int first, int second)
{
    int result = -1;
    if (!self->active) {
        return result;
    }
    if (self->state != result) {
        return -2;
    }
    func_089F84E0(self->field_14);
    func_089F85B0(self->field_14, value, 0);
    self->state = 0;
    self->elapsed = 0.0f;
    self->field_0C = first;
    self->field_10 = second;
    return 0;
}
