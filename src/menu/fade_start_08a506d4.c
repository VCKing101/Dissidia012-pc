typedef struct FadeState {
    unsigned int field_00;
} FadeState;

typedef struct FadeOwner {
    char pad_00[0x58];
    FadeState *field_58;
} FadeOwner;

typedef struct FadeColor {
    float r;
    float g;
    float b;
    float a;
} FadeColor;

extern int func_088D4C5C(void);
extern void func_08873D34(int layer, void (*done)(void), int a, int b, int c, float frames);
extern void func_08873DB0(FadeColor *color, int layer);
extern void func_08A507C8(void);

void func_08A506D4(FadeOwner *self)
{
    FadeColor color;
    FadeState *state = self->field_58;
    int frames;
    state->field_00 = state->field_00 | 1;
    frames = func_088D4C5C();
    func_08873D34(2, func_08A507C8, 0, 1, 2, (float)frames);
    color.r = 0.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 0.0f;
    func_08873DB0(&color, 2);
}
