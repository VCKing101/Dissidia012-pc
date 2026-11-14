/* Sets a definition reference; a shared reference points at the reset global definition. */
typedef struct Def088C1520 {
    char data[0x30];
    short state;
} Def088C1520;

typedef struct DefRef088C1520 {
    int key;
    void *def;
} DefRef088C1520;

extern Def088C1520 D_09B69270;
void func_088C6F68(Def088C1520 *def);
void func_088C1520(DefRef088C1520 *ref, int key, void *def, unsigned char shared)
{
    ref->key = key;
    ref->def = def;
    if (shared == 1) {
        func_088C6F68(&D_09B69270);
        D_09B69270.state = 2;
        ref->def = &D_09B69270;
    }
}
