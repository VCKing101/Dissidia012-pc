typedef struct Actor0883809C {
    unsigned char unknown_000[0x180];
    short motion;
} Actor0883809C;

int func_08838074(Actor0883809C *self);

int func_0883809C(Actor0883809C *self) {
    short motion = self->motion;
    if (motion == 0x2BE || motion == 0x2BF || motion == 0x2C0 || motion == 0x2F0 || motion == 0x2F1 || motion == 0x2F2) {
        return 0;
    }
    return func_08838074(self);
}
