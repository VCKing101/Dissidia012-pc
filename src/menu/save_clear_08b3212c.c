/* Clears the 0x12C-byte save block at D_09B6DD10 + 8. */
struct Save08B3212C {
    unsigned char field_0;
    char data[0x12C - 1];
};

struct Global08B3212C {
    char pad_0[8];
    struct Save08B3212C field_8;
};

extern struct Global08B3212C D_09B6DD10;
extern void func_08935828(void *dst, int value, int size);

void func_08B3212C(void)
{
    func_08935828(&D_09B6DD10.field_8, 0, 0x12C);
    D_09B6DD10.field_8.field_0 = 0;
}
