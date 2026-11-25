/* Dispatches on the state returned by the owner's query: finishes one step (state 0x19) or runs the three-part completion (state 0x1B). */
typedef struct C3FC68 { int state; char pad0[0x10 - 4]; int p10; char pad1[0x1C - 0x14]; int sel; char pad2[0x24 - 0x20]; int w24; } C3FC68;
typedef struct A3FC68 { char pad0[8]; int q; char pad1[0x24 - 0xC]; int e; char pad2[0x2C - 0x28]; int g; } A3FC68;
typedef struct S3FC68 { char pad0[0xC4]; C3FC68 *c; A3FC68 *a; } S3FC68;
typedef struct G3FC68 { char pad[4]; short id; } G3FC68;
extern G3FC68 D_09B6DAC0;
extern int func_08B51874(int);
extern void func_08B4E854(int);
extern void func_08B3A1CC(S3FC68 *, int);
extern void func_08B4E794(int);
extern void func_08B527E8(int);
extern void func_08B3A008(S3FC68 *, unsigned char);
extern void func_08B46330(int);
void func_08B3FC68(S3FC68 *self)
{
    switch (func_08B51874(self->a->q)) {
    case 0:
        break;
    case 1:
    case 2:
        func_08B4E854(self->a->e);
        self->c->state = 0x19;
        break;
    case 3:
        switch (self->c->sel) {
        case 2: {
            G3FC68 *g = &D_09B6DAC0;
            func_08B3A1CC(self, g->id);
            g->id = -1;
        }
        case 1:
            func_08B4E794(self->a->e);
            func_08B527E8(self->a->g);
            func_08B3A008(self, 1);
            func_08B46330(self->c->p10);
            self->c->w24 = 1;
            self->c->state = 0x1B;
            break;
        }
        break;
    }
}
