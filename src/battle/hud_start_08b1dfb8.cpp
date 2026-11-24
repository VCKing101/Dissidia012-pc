// Starts the HUD element's animation chosen by the current mode and sets state 3; -1/-2 when its parts are missing.
extern "C" {
typedef struct Parts { char pad[0x5A0]; int anim; } Parts;
typedef struct Hud { char pad0[4]; int state; char pad1[4]; Parts *parts; char pad2[0x2C - 0x10]; int handle; } Hud;
typedef struct Game { char pad0[0x4C]; int kind; char pad1[0x740 - 0x50]; int mode; } Game;
extern void func_088DE6D4(int, int, int, int);
extern Game *D_089D6A10;
struct Cmp {
    static bool Is(int v, int c) { return v == c; }
};
int func_08B1DFB8(Hud *self) {
    int anim;
    int k;
    int mode;
    if (self->parts == 0) return -1;
    anim = self->parts->anim;
    if (anim == 0) return -2;
    mode = D_089D6A10->mode;
    if (Cmp::Is(mode, 1)) {
        func_088DE6D4(self->handle, anim, 3, 1);
    } else {
        if (Cmp::Is(mode, 2) && D_089D6A10->kind != 0xC) {
            func_088DE6D4(self->handle, anim, 0x39, 1);
        } else if ((k = D_089D6A10->kind) == 10 || k == 0xC) {
            func_088DE6D4(self->handle, anim, 0x3D, 1);
        } else {
            func_088DE6D4(self->handle, anim, 3, 1);
        }
    }
    self->state = 3;
    return 0;
}
}
