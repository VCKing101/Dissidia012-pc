typedef struct Wk {
    char pad[0x84]; int f84; int f88; int f8C; int f90; int f94; int f98; int f9C; int fA0; int fA4; int fA8; int fAC; int fB0; int fB4;
    char padB8[0xD0 - 0xB8]; int fD0; char padD4[0xF4 - 0xD4]; int fF4; char padF8[0x190 - 0xF8]; int f190;
} Wk;
typedef struct Sc { Wk *wk; } Sc;
extern void func_088D9CD8(int);
extern void func_088D9CFC(int);
extern void func_0887B924(int);
extern void func_0887B944(int);
extern void func_08B255A0(Sc *);
void func_08B27D34(Sc *self, unsigned char mode)
{
    if (mode == 1) {
        func_088D9CD8(self->wk->fD0);
        func_088D9CD8(self->wk->fB0);
        func_088D9CD8(self->wk->f94);
        func_088D9CD8(self->wk->f98);
        func_088D9CD8(self->wk->f9C);
        func_088D9CD8(self->wk->fA0);
        func_088D9CD8(self->wk->f84);
        func_088D9CD8(self->wk->f88);
        func_088D9CD8(self->wk->f8C);
        func_088D9CD8(self->wk->f90);
        func_088D9CD8(self->wk->fB4);
        func_088D9CD8(self->wk->fA4);
        func_088D9CD8(self->wk->fA8);
        func_088D9CD8(self->wk->fAC);
        func_0887B924(self->wk->fF4);
        func_0887B924(self->wk->f190);
        func_08B255A0(self);
    } else {
        func_088D9CFC(self->wk->fD0);
        func_088D9CFC(self->wk->fB0);
        func_088D9CFC(self->wk->f94);
        func_088D9CFC(self->wk->f98);
        func_088D9CFC(self->wk->f9C);
        func_088D9CFC(self->wk->fA0);
        func_088D9CFC(self->wk->f84);
        func_088D9CFC(self->wk->f88);
        func_088D9CFC(self->wk->f8C);
        func_088D9CFC(self->wk->f90);
        func_088D9CFC(self->wk->fB4);
        func_088D9CFC(self->wk->fA4);
        func_088D9CFC(self->wk->fA8);
        func_088D9CFC(self->wk->fAC);
        func_0887B944(self->wk->fF4);
        func_0887B944(self->wk->f190);
    }
}
