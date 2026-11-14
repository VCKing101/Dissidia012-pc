typedef struct Bank08885AD4 {
    unsigned char unknown_00[0x10];
    int *voices;
} Bank08885AD4;

extern Bank08885AD4 *D_089D69E8;
extern int D_089D7830;
void func_088CFA94(int system, int voice, int arg);

void func_08885AD4(int arg) {
    int voice = *D_089D69E8->voices;
    func_088CFA94(D_089D7830, voice, arg);
}
