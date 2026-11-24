/* Sets the state word at self->p14C+0xA5C from func_08ABC7C8 or func_08ABC75C (depending on D_089D5F20) when byte +0xB47 is set; else to 0x21 if func_088AC9A8(D_089D712C). */
typedef struct { char p0[0xA5C]; int state; } Blk0;
typedef struct { char p0[0xB47]; unsigned char flag; } Blk1;
typedef struct { char p0[0x14C]; char *p14C; } Obj;
typedef struct { char p0[0xA5C]; int state; char p1[0xB47 - 0xA60]; unsigned char flag; } Blk;
typedef struct { char p0[0x14C]; Blk *blk; } O2;
extern unsigned char D_089D5F20;
extern int D_089D712C;
extern int func_088AC9A8(int a);
extern int func_08ABC75C(O2 *o);
extern int func_08ABC7C8(O2 *o);
void func_08ABCEEC(O2 *self) {
    if (self->blk->flag != 0) {
        if (D_089D5F20 != 0) {
            self->blk->state = func_08ABC7C8(self);
        } else {
            self->blk->state = func_08ABC75C(self);
        }
    } else {
        if (func_088AC9A8(D_089D712C) != 0) {
            self->blk->state = 0x21;
        }
    }
}
