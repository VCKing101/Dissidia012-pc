// Calls func_08B41A20 with the second word of the 8-byte table entry (base 0x08B42290) selected by the word at +0xC.
typedef struct { int a; int b; } Pair08B419F0;
extern Pair08B419F0 D_08B42290[];
extern void func_08B41A20(void *, void *);
typedef struct { int pad[3]; int idx; } Obj08B419F0;
void func_08B419F0(Obj08B419F0 *self)
{
    func_08B41A20(self, &D_08B42290[self->idx].b);
}
