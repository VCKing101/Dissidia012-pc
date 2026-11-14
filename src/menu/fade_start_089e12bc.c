/* Starts a fade towards opaque white and moves the owner's state to 5. */
typedef struct FadeColor {
    float r;
    float g;
    float b;
    float a;
} FadeColor;

typedef struct FadeOwner {
    char pad_000[0x140];
    int *field_140;
    char field_144[4];
} FadeOwner;

extern int func_088D4C5C(void);
extern void func_08873D34(float time, int kind, void *callback, int arg, int one, int two);
extern void func_08873DB0(FadeColor *color, int mode);
extern void func_088D54DC(void *timer);
extern char D_089E0AA4[];

void func_089E12BC(FadeOwner *self)
{
    FadeColor color;
    func_08873D34((float)func_088D4C5C(), 3, D_089E0AA4, 0, 1, 2);
    color.r = 255.0f;
    color.g = 255.0f;
    color.b = 255.0f;
    color.a = 255.0f;
    func_08873DB0(&color, 2);
    func_088D54DC(self->field_144);
    *self->field_140 = 5;
}
