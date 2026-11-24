/* Stores a handle at +0x10, creates a child with two callbacks via func_088823C4, stores it at +0x14 and links it back to self at +4. */
typedef struct Child { int pad; int owner; } Child;
extern Child *func_088823C4(int a, int b, int c, void (*f)(void), void (*g)(void));
extern void D_08B46DF4(void);
extern void D_08B48498(void);
void func_08B45DBC(int *self, int a, int b, int c)
{
    Child *ch;
    self[4] = a;
    ch = func_088823C4(a, b, c, D_08B46DF4, D_08B48498);
    self[5] = (int)ch;
    ch->owner = (int)self;
}
