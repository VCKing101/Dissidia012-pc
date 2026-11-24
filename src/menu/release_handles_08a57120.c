/* Frees three handles, then creates a new handle from the selected entry and derives the other two from it. */
typedef struct Hs {
    char pad0[0x5C]; int h5c; int h60; int h64;
    char pad1[0x104 - 0x68]; int sel;
    char pad2[0x154 - 0x108]; int tbl[1];
} Hs;
extern int D_089D6998;
extern void func_0887BA5C(int);
extern int func_0887D868(int, int, int, int, int, int, int, int);
extern int func_0887E534(int);
extern void func_088D428C(Hs *, int *, int *, int);
void func_08A57120(Hs *self) {
    int sel = self->sel;
    if (self->h5c) { func_0887BA5C(self->h5c); self->h5c = 0; }
    if (self->h60) { func_0887BA5C(self->h60); self->h60 = 0; }
    if (self->h64) { func_0887BA5C(self->h64); self->h64 = 0; }
    self->h5c = func_0887D868(D_089D6998, 0, self->tbl[sel], -1, 0, 0, 0, 0);
    self->h60 = func_0887E534(self->h5c);
    self->h64 = func_0887E534(self->h5c);
    func_088D428C(self, &self->h5c, (int *)((char *)self + 0x8C), 1);
}
