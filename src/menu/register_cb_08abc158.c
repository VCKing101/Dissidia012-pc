/* Stores the arguments in the global block at 0x09B6D9D8, registers two callbacks on self + 0x100 and attaches self to it. */
extern struct Blk { int a, b, c; char d0, d1; char pad[2]; int d; } D_09B6D9D8;
extern void func_0886503C(int p, void *a, void *b, int c);
extern void func_0886504C(int p, int owner, int size);
extern void func_08ABF408(void);
extern void func_08ABF428(void);
void func_08ABC158(int self, int b, int c, int d) {
    *(int *)(self + 0x10C) = b;
    D_09B6D9D8.c = c;
    D_09B6D9D8.d1 = c == 1;
    D_09B6D9D8.d = d;
    func_0886503C(self + 0x100, (void *)func_08ABF408, (void *)func_08ABF428, 0);
    func_0886504C(self + 0x100, self, 0x100);
}
