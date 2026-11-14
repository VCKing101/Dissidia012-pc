typedef struct SoundNode {
    char pad_000[0x130];
    void *sound;
} SoundNode;

extern void func_08808214(void *sound);

void func_08AD7E48(SoundNode *self)
{
    if (self->sound != 0) {
        func_08808214(self->sound);
        self->sound = 0;
    }
}
