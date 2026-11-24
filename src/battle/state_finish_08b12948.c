/* Finishes the linked object's state: runs cleanup calls when flagged, drops its pending handle, sets state 5. */
typedef struct FinLinked { char pad_00[0x884]; int handle; int token; int state; char pad_890[2]; unsigned char flag; } FinLinked;
typedef struct FinSelf { char pad_00[4]; int owner; FinLinked *linked; } FinSelf;
extern int D_089D6C80;
extern void func_08B125D8(FinSelf *self);
extern void func_088D3A04(FinLinked *l);
extern void func_08B12630(FinSelf *self);
extern void func_088A0DFC(int v);
extern void func_0885C538(int owner, int handle, int z);
void func_08B12948(FinSelf *self)
{
    if (self->linked->flag != 0) {
        func_08B125D8(self);
        func_088D3A04(self->linked);
        func_08B12630(self);
    }
    if (self->linked->token != -1) {
        func_088A0DFC(D_089D6C80);
    }
    if (self->linked->handle != 0) {
        func_0885C538(self->owner, self->linked->handle, 0);
        self->linked->handle = 0;
    }
    self->linked->state = 5;
}
