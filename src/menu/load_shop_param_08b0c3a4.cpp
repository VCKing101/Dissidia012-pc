// Loads the shop parameter file into a freshly allocated block and records its start and the data pointer at +0x14.
struct Shared { char pad[0x24754]; char *block; char *start; char *data; };
struct Owner { char pad[0x134]; int heap; Shared *shared; };
extern char D_08B1C900[];
extern "C" {
extern int func_08862C40(char *, int);
extern char *func_0885C6CC(int, int, int, int, int, int);
extern void func_08862854(char *, char *, int, int);
void func_08B0C3A4(Owner *self) {
    int size = func_08862C40(D_08B1C900, 1);
    int one = 1;
    self->shared->block = func_0885C6CC(self->heap, size, 0, 0, one, 0);
    func_08862854(D_08B1C900, self->shared->block, size, 1);
    char *b = self->shared->block;
    self->shared->start = b;
    self->shared->data = b + 0x14;
}
}
