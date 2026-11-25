/* Initialises a 0x48C-byte record: header (id 9, mode 2), two 8-entry tables of -1, the 0x1E slot pointers/halves, and the data pointer. */
typedef struct R08926D2C {
    short id; unsigned char b2; unsigned char b3; unsigned char b4; unsigned char b5; short h6;
    char pad[0x480 - 8]; int w480; int w484; int w488;
} R08926D2C;
int func_0892615C(void *o);
int func_0892614C(void *o);
void func_08926D2C(void *self, R08926D2C *out)
{
    int i;
    int *p;
    int v;
    int base;
    int neg;

    out->id = 9;
    out->b3 = 0;
    out->b4 = 0;
    out->h6 = 2;
    out->b5 = 0;
    out->w484 = 0;
    for (i = 0, neg = -1, p = (int *)out; i < 8; i++, p++) {
        p[2] = neg;
        p[10] = neg;
    }
    base = func_0892615C(self);
    v = base + 0x74;
    for (i = 0, p = (int *)out, v += 0x48C; i < 0x1E; i++, p++, v -= 4) {
        p[0x408 / 4] = v;
        *(short *)(v + (int)out) = 0;
    }
    out->w480 = base + func_0892614C(self) + 0x48C;
    out->b2 = 0;
    out->w488 = 0;
}
