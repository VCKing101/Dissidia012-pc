typedef struct Slot0887B13C {
    void *block;
    unsigned char used;
} Slot0887B13C;

typedef struct Pool0887B13C {
    int count;
    char *blocks;
    Slot0887B13C slots[8];
} Pool0887B13C;

extern Pool0887B13C D_09B48C18;
extern void *D_089D6998;
extern char D_09B49088[];
extern char D_089C9D40[];
extern char *func_088820D8(int size);
extern void func_0887DF14(void *heap, void *area, int size, const char *name);

void func_0887B13C(void)
{
    int i;
    char *blocks;
    char *p;

    D_09B48C18.count = 0;
    blocks = func_088820D8(0x80);
    D_09B48C18.blocks = blocks;
    for (i = 0, p = blocks; i < 8; p += 0x10, i++) {
        D_09B48C18.slots[i].block = p;
        D_09B48C18.slots[i].used = 0;
    }
    func_0887DF14(D_089D6998, D_09B49088, 0x80, D_089C9D40);
}
