struct Masks {
    unsigned long long bits;
    char data_08[0x44 - 0x08];
    unsigned int flags;   /* 0x44 */
    static bool pending(Masks *m) { return (m->flags & 2) != 0; }
};
extern "C" {
extern unsigned long long D_089C9A68;
int func_08872594(Masks *m);
void func_08871B40(Masks *m, unsigned long long keep);
unsigned long long func_088726B0(Masks *m);
void func_08871BD0(Masks *m, unsigned long long bits);

void func_088725A0(Masks *m)
{
    if (func_08872594(m) != 0) {
        func_08871B40(m, D_089C9A68);
        m->flags |= 2;
    } else {
        unsigned long long bits = func_088726B0(m);
        if (Masks::pending(m)) {
            m->bits = bits;
        }
        func_08871BD0(m, bits);
        m->flags &= ~2;
    }
}
}
