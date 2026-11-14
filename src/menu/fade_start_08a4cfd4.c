/* Starts a fade over the given frames (the default length when not positive) to black, and flags it (2). */
typedef struct FadeState {
    unsigned int field_00;
} FadeState;

typedef struct FadeOwner {
    char pad_00[0x5C];
    FadeState *field_5C;
} FadeOwner;

extern int func_088D4C5C(void);
extern void func_08873D34(float frames, int layer, void (*done)(void), int a, int b, int c);
extern void func_08873DB0(float *color, int layer);
extern void func_08A4D5D0(void);

void func_08A4CFD4(FadeOwner *self, float frames)
{
    float color[4];
    int length = frames;
    if (length <= 0.0f) {
        length = func_088D4C5C();
    }
    func_08873D34(length, 2, func_08A4D5D0, 0, 1, 2);
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 0.0f;
    func_08873DB0(color, 2);
    self->field_5C->field_00 |= 2;
}
