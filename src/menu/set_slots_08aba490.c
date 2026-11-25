typedef struct Rec { short s[20]; } Rec;
typedef struct Blk { char pad[0x8BC]; int w8BC; char pad2[0x9E8 - 0x8C0]; Rec rec[5]; char pad3[0xD34 - 0xAB0]; int wD34; } Blk;
typedef struct Wk { char pad[0x138]; Blk *b; } Wk;
extern int func_08AB9B98(Wk *);
extern void func_08AB8768(Wk *, void *);
extern int func_08AB8128(Wk *, int);
extern void func_088D428C(void *, void *, void *, int);
void func_08ABA490(Wk *self)
{
    int i;
    for (i = 0; i < func_08AB9B98(self); i++) {
        int r;
        int x;
        int y;
        func_08AB8768(self, &self->b->rec[i]);
        r = func_08AB8128(self, self->b->w8BC + i);
        if (r == 0) {
            x = i * self->b->wD34;
            y = i + 0xD;
            self->b->rec[i].s[0] = x + 1;
            self->b->rec[i].s[1] = 9;
            self->b->rec[i].s[2] = y;
            self->b->rec[i].s[3] = 1;
            self->b->rec[i].s[4] = x;
            self->b->rec[i].s[5] = 9;
            self->b->rec[i].s[6] = y;
            self->b->rec[i].s[7] = 1;
            self->b->rec[i].s[8] = x + 3;
            self->b->rec[i].s[9] = 0x12;
            self->b->rec[i].s[10] = y;
            self->b->rec[i].s[11] = 0;
            self->b->rec[i].s[12] = x + 2;
            self->b->rec[i].s[13] = 0x12;
            self->b->rec[i].s[14] = y;
            self->b->rec[i].s[15] = 0;
            func_08AB8768(self, &self->b->rec[i].s[16]);
        }
        func_088D428C((char *)self->b + 0x400, (char *)self->b + 0x900, &self->b->rec[i], 1);
    }
}
