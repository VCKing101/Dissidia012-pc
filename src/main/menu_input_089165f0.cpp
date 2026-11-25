/* Menu input: key A (0x2712 cue) latches flag 0x7D8; key B asks func_088DE7E8 for the choice (0: cue 0x2711 and refresh; 1: cue 0x2712 and latch). */
struct Input08916 { char d[0x34]; virtual int v0(unsigned long long k); virtual int v1(unsigned long long k); };
struct Obj089165F0 {
    char pad0[0x31C]; int sub; char pad1[0x7D8 - 0x320]; unsigned char latch;
};
extern "C" {
extern unsigned long long D_089CE350;
extern unsigned long long D_089CE370;
Input08916 *func_088729A0(int a);
void func_088A24D4(int a, int b, int c, int d);
void func_08916748(Obj089165F0 *o);
void func_0891386C(Obj089165F0 *o);
int func_088DE7E8(int *p);
void func_088CE7F4(void *p);
void func_089160F0(Obj089165F0 *o);
void func_08916068(Obj089165F0 *o);
void func_089165F0(Obj089165F0 *self)
{
    if (func_088729A0(0)->v1(D_089CE350) != 0) {
        func_088A24D4(0, 0x2712, 1, 0);
        func_08916748(self);
        self->latch = 1;
        func_0891386C(self);
    } else if (func_088729A0(0)->v1(D_089CE370) != 0) {
        switch (func_088DE7E8(&self->sub)) {
        case 0:
            func_088A24D4(0, 0x2711, 1, 0);
            func_08916748(self);
            func_088CE7F4((char *)self + 0x404);
            func_089160F0(self);
            func_08916068(self);
            break;
        case 1:
            func_088A24D4(0, 0x2712, 1, 0);
            func_08916748(self);
            self->latch = 1;
            func_0891386C(self);
            break;
        }
    }
}
}
