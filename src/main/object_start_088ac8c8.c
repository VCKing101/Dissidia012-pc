/* Waits for the global gate, then starts the object when asked or when its flag is set. */
typedef struct G { char pad[3]; unsigned char busy; } G;
typedef struct O { char pad[0xEF8]; unsigned flags; unsigned char c; } O;
extern G *D_089D7280;
extern int D_089D69E0;
extern unsigned char D_089D5F1F;
extern void func_08883F9C(int, int, int);
extern void func_088AD460(O *);
extern int func_088AC1AC(O *, int, int);
int func_088AC8C8(O *self, unsigned char force)
{
    while (D_089D7280->busy != 0) {
        func_08883F9C(D_089D69E0, 1, 0);
    }
    if (D_089D5F1F != 0) return 1;
    if (force == 0 && (self->flags & 0x2000) == 0) return 0;
    self->flags = self->flags & 0xFFFFDFFF;
    func_088AD460(self);
    self->flags = self->flags | 1;
    self->flags = self->flags | 2;
    return func_088AC1AC(self, 1, self->c);
}
