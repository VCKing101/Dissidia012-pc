/* Rolls a random 15-bit seed into +4, initialises two sub-objects and clears +8. */
typedef struct Obj { int a; unsigned seed; int cleared; char pad[0x54]; char subA[0x44]; char subB[1]; } Obj;
extern int D_09AF84D0;
extern void func_08B16DF4(void *p);
extern void func_08B09668(void *p);

void func_08B05CCC(Obj *self)
{
    self->seed = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    func_08B16DF4(self->subA);
    func_08B09668(self->subB);
    self->cleared = 0;
}
