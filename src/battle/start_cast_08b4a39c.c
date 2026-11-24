// Starts a cast: needs the owner at +0 and a unit from func_08916FA0 and a positive count read via func_089200D8; runs the setup calls on +0x25C and +0x308 and returns 1, else 0.
extern int func_08916FA0(void);
extern int *func_089200D8(int);
extern void func_08B4A14C(void *, int, int);
extern void func_0892D380(void *, int, int, int);
extern void func_0893148C(void *, int, int, int, int);
extern int D_089D712C;
typedef struct Cast57 {
    int owner;
    char pad0[0x25C - 4];
    char sub1[0xAC];
    char sub2[1];
} Cast57;
typedef struct Unit57 {
    char pad0[0x5A0];
    int anim;
} Unit57;
int func_08B4A39C(Cast57 *self, unsigned char a, unsigned char b)
{
    int la = a;
    int lb = b;
    Unit57 *u;
    int *cnt;
    if (self->owner == 0) {
        return 0;
    }
    u = (Unit57 *)func_08916FA0();
    if (u == 0) {
        return 0;
    }
    cnt = func_089200D8(D_089D712C + 0x3F548);
    if (*cnt <= 0) {
        return 0;
    }
    func_08B4A14C(self, 0, lb);
    func_0892D380(self->sub1, self->owner, 2, 0);
    func_0893148C(self->sub2, u->anim, 0, la, lb);
    return 1;
}
