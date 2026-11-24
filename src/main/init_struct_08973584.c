// Zeroes the fields of a record (leaving word 0xC), sets the byte at +0x44 to 1, and returns it.
struct T { int w[12]; int skip; int a; int b; unsigned char c; char pad[3]; int d; unsigned char e; };
struct T *func_08973584(struct T *t) {
    t->w[0] = 0;
    t->w[1] = 0;
    t->w[2] = 0;
    t->w[3] = 0;
    t->w[4] = 0;
    t->w[5] = 0;
    t->w[6] = 0;
    t->w[7] = 0;
    t->w[8] = 0;
    t->w[9] = 0;
    t->w[10] = 0;
    t->w[11] = 0;
    t->a = 0;
    t->b = 0;
    t->c = 0;
    t->d = 0;
    t->e = 1;
    return t;
}
