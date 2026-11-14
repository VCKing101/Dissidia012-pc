/* Starts a fade to white over the given frames (default length when not positive) unless one runs; optional sound. */
typedef struct FadeState {
    unsigned int field_00;
} FadeState;

typedef struct FadeOwner {
    char pad_00[0x4C];
    char field_4C[0x10];
    FadeState *field_5C;
} FadeOwner;

extern int func_088D4C5C(void);
extern int func_08873EB4(void);
extern void func_08873D34(float frames, int layer, void (*done)(void), int a, int b, int c);
extern void func_08873DB0(float *color, int layer);
extern void func_08A4D5F0(void);
extern void func_088D54DC(char *sound);

void func_08A4D07C(FadeOwner *self, float frames, unsigned char play)
{
    float color[4];
    int length = frames;
    if (length <= 0.0f) {
        length = func_088D4C5C();
    }
    if (func_08873EB4() == 0) {
        func_08873D34(length, 3, func_08A4D5F0, 0, 1, 2);
        color[0] = 255.0f;
        color[1] = 255.0f;
        color[2] = 255.0f;
        color[3] = 255.0f;
        func_08873DB0(color, 2);
        self->field_5C->field_00 |= 4;
    }
    if (play) {
        func_088D54DC(self->field_4C);
    }
}
