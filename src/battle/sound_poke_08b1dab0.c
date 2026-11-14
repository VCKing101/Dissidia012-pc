typedef struct SoundRef {
    int field_00;
    void *child;    /* 0x04 */
} SoundRef;

extern int func_08B1DB00(void *child);

void func_08B1DAB0(SoundRef *ref)
{
    if (ref == 0) {
        return;
    }
    if (ref->child == 0) {
        return;
    }
    if (func_08B1DB00(ref->child) != 0) {
        return;
    }
}
