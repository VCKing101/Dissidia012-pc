// Ends the scene: with no owner (+0xC) returns -1; otherwise clears the byte at 089D6C98, restores the globals via func_088A2780/0889661C/088A18B0, ORs the 64-bit mask 08B1FCB8 into the owner's flags (+0x388), fires func_08889944, sets state 5 and returns 0.
extern void func_088A2780(void);
extern void func_0889661C(int);
extern void func_088A18B0(int, int, int);
extern void func_08889944(int, int);
extern unsigned char D_089D6C98;
extern int D_089D6AE8;
extern int D_089D6C88;
extern int D_089D6A10;
extern float D_089C9F54;
extern unsigned long long D_08B1FCB8;
typedef struct Owner57 {
    char pad0[0x388];
    unsigned long long bits;
} Owner57;
typedef struct Scene57 {
    int pad0;
    int state;
    int pad1;
    Owner57 *owner;
} Scene57;
int func_08B1E16C(Scene57 *self)
{
    Scene57 *keep[1];
    int zero;
    if (self->owner == 0) {
        return -1;
    }
    keep[0] = self;
    func_088A2780();
    zero = 0;
    D_089D6C98 = zero;
    func_0889661C(D_089D6AE8);
    func_088A18B0(D_089D6C88, 100, (int)D_089C9F54);
    self = keep[0];
    self->owner->bits = self->owner->bits | D_08B1FCB8;
    func_08889944(D_089D6A10, 8);
    self = keep[0];
    self->state = 5;
    return 0;
}
