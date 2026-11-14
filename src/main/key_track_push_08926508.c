typedef struct KeyPair { short a; short b; } KeyPair;
typedef struct KeyTrack { char pad_00[2]; unsigned char count; char pad_03[0x480 - 3]; int key_offset; } KeyTrack;
unsigned char func_08926508(void *self, KeyTrack *t, short a, short b)
{
    unsigned int n = t->count;
    KeyPair *keys = (KeyPair *)(t->key_offset + (int)t);
    unsigned char old;
    keys[n].a = a;
    keys[t->count].b = b;
    old = t->count;
    t->count = old + 1;
    return old;
}
