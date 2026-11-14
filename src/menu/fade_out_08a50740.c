/* Starts the closing fade unless one is running. */
typedef struct FadeOutColor {
    float r;
    float g;
    float b;
    float a;
} FadeOutColor;

typedef struct FadeOutOwner {
    char pad_00[0x58];
    unsigned int *field_58;
} FadeOutOwner;

extern int func_08873EB4(void);
extern int func_088D4C5C(void);
extern void func_08873D34(float time, int kind, void *callback, int arg, int one, int two);
extern void func_08873DB0(FadeOutColor *color, int mode);
extern char D_08A507E8[];

void func_08A50740(FadeOutOwner *self)
{
    FadeOutColor color;
    if (func_08873EB4() == 0) {
        *self->field_58 = *self->field_58 | 2;
        func_08873D34((float)func_088D4C5C(), 3, D_08A507E8, 0, 1, 2);
        color.r = 255.0f;
        color.g = 255.0f;
        color.b = 255.0f;
        color.a = 255.0f;
        func_08873DB0(&color, 2);
    }
}
