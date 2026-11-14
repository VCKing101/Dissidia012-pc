typedef struct SoundNode {
    char pad_000[0x100];
    void *sound;
} SoundNode;

extern void func_08808214(void *sound);

void func_08ADA06C(SoundNode *self)
{
    if (self->sound != 0) {
        func_08808214(self->sound);
        self->sound = 0;
    }
}
