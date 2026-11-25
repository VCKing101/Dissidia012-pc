typedef struct Self37E10 { char pad[0xF64]; int *sub; } Self37E10;
extern char D_08A38F5C[];
extern void func_088D3EA0(int, int, int);
extern void func_088D4664(int, int *, int);
extern void func_088D428C(int, int *, int, int);
extern void func_08A365D4(void *);
extern void func_08A363E4(void *, int);
extern void func_08A37AC8(void *, int);
extern void func_08A360B0(void *, int, int, int);
extern void func_08A36108(void *, int, int, int);
extern void func_08A36160(void *, int, int, int);
extern void func_08A361D4(void *, int, int, int);
extern void func_08A36248(void *, int, int, int);
extern void func_08A362EC(void *, int, int, int);
extern void func_08A36358(void *, int, int, int);

void func_08A37E10(Self37E10 *self, unsigned char flag)
{
    int *p;
    int *q;
    int k;
    int k2;
    p = self->sub;
    k = p[0x1B];
    q = p + 3;
    if (p[0x47] != 0) {
        func_088D3EA0(*p, p[1], p[0x47]);
        p = self->sub;
    }
    if (p[0x48] != 0) {
        func_088D4664(p[1], q, p[0x48]);
    }
    func_08A365D4(self);
    func_08A363E4(self, k);
    if (flag != 0) {
        func_08A37AC8(self, self->sub[0x1B]);
    }
    self->sub[0x48] = (int)D_08A38F5C;
    func_088D428C(self->sub[1], q, self->sub[0x48], 1);
    k2 = k * 2;
    func_08A360B0(self, (int)self->sub + 0x464, k, k2);
    func_08A36108(self, (int)self->sub + 0x47C, k, k2);
    func_08A36160(self, (int)self->sub + 0x494, k, k2);
    func_08A361D4(self, (int)self->sub + 0x4AC, k, k2);
    func_08A36248(self, (int)self->sub + 0x4C4, k, k2);
    func_08A362EC(self, (int)self->sub + 0x4DC, k, k2);
    func_08A36358(self, (int)self->sub + 0x4F4, k, k2);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x464, 1);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x47C, 0);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x494, 1);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x4AC, 0);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x4C4, 0);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x4DC, 0);
    func_088D428C(self->sub[1], q, (int)self->sub + 0x4F4, 0);
}
