// When enabled (+0x10), queries the first scene object with a virtual key test (slot 1, key D_08B46A68); on a hit plays the sound at the id (+4->+0x1C), then starts func_0891166C on +0x30 for 4.0; returns 1.
struct Scene57 {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
extern "C" {
extern Scene57 *func_088729A0(int);
extern void func_089170B8(int);
extern void func_0891166C(int, int, int, float);
extern unsigned long long D_08B46A68;
extern int D_089D83A8;
}
struct Info57 {
    char pad0[0x1C];
    short id;
};
struct Self57 {
    char pad0[4];
    Info57 *info;
    char pad1[8];
    unsigned char enabled;
    char pad2[0x1F];
    int target;
};
extern "C" int func_08B468C0(Self57 *self)
{
    if (self->enabled != 0) {
        if (func_088729A0(0)->slot1(D_08B46A68) != 0) {
            func_089170B8(self->info->id);
        }
        func_0891166C(D_089D83A8, self->target, 6, 4.0f);
    }
    return 1;
}
