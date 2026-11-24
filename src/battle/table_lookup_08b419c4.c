// Calls func_08B41A20 with the 8-byte table entry (base 0x08B42290) selected by the word at +0xC.
extern char D_08B42290[];
extern void func_08B41A20(void *, void *);
typedef struct { int pad[3]; int idx; } Obj08B419C4;
void func_08B419C4(Obj08B419C4 *self)
{
    func_08B41A20(self, D_08B42290 + self->idx * 8);
}
