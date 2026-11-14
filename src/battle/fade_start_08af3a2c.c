typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct FadeOwner {
    char pad_00[0x5C];
    int state;
} FadeOwner;

extern int func_088D4C5C(void);
extern void func_08873D34(int mode, void (*callback)(FadeOwner *), FadeOwner *owner, int d, int e, float time);
extern void func_08873DB0(Vec4 *color, int mode);
extern void func_08AF3ABC(FadeOwner *owner);

void func_08AF3A2C(FadeOwner *self)
{
    Vec4 color;
    float time = (float)func_088D4C5C();
    func_08873D34(3, func_08AF3ABC, self, 0, 2, time);
    color.x = 255.0f;
    color.y = 255.0f;
    color.z = 255.0f;
    color.w = 255.0f;
    func_08873DB0(&color, 2);
    self->state = 5;
}
