typedef struct Bank08885A9C {
    unsigned char unknown_00[0x10];
    int *voices;
} Bank08885A9C;

extern Bank08885A9C *D_089D69E8;
extern int D_089D7830;
void func_088CFA94(int system, int voice, int arg);

void func_08885A9C(int arg) {
    int voice = *D_089D69E8->voices;
    func_088CFA94(D_089D7830, voice, arg);
}
