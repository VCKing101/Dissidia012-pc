typedef struct Slot0887B648 {
    void *block;
    unsigned char used;
} Slot0887B648;

typedef struct Pool0887B648 {
    int count;
    char *blocks;
    Slot0887B648 slots[4];
} Pool0887B648;

extern Pool0887B648 D_09B48C60;
extern char *func_088820D8(int size);

void func_0887B648(void)
{
    int i;
    char *blocks;

    D_09B48C60.count = 0;
    blocks = func_088820D8(0x20);
    D_09B48C60.blocks = blocks;
    char *p;
    for (i = 0, p = blocks; i < 4; p += 8, i++) {
        D_09B48C60.slots[i].block = p;
        D_09B48C60.slots[i].used = 0;
    }
}
