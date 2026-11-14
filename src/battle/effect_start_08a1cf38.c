typedef struct EffectNode {
    char pad_00[0x20];
    unsigned short flags;
} EffectNode;

typedef struct EffectStart {
    char pad_00[0x8];
    int state;
    unsigned char ready;
    char pad_0D[0x3];
    float elapsed;
    char pad_14[0x28];
    char field_3C[0x28];
    char field_64[0x4];
} EffectStart;

extern void func_08A01AE0(void *sub, int mode, int count);
extern void func_08A01D60(void *sub);
extern EffectNode *func_08A01AC0(void *sub);

int func_08A1CF38(EffectStart *self)
{
    if (self->ready == 1) {
        EffectNode *node;
        func_08A01AE0(self->field_3C, 1, -1);
        func_08A01AE0(self->field_64, 5, -1);
        func_08A01D60(self->field_64);
        func_08A01D60(self->field_3C);
        node = func_08A01AC0(self->field_64);
        if (node != 0) {
            node->flags &= ~2;
        }
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}
