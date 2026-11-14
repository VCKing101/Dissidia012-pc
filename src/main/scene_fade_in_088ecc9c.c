typedef struct Color { float r; float g; float b; float a; } Color;
typedef struct Scene { char data_00[0x2C]; int state; } Scene;
extern int func_088D4C5C(void);
extern void func_08873D34(float time, int mode, void (*done)(Scene *s), Scene *s, int a, int b);
extern void func_08873DB0(Color *c, int mode);
extern void func_088ECD98(Scene *s);

void func_088ECC9C(Scene *s)
{
    Color c;
    int time = func_088D4C5C();
    int state = 3;
    func_08873D34((float)time, 3, func_088ECD98, s, 0, 2);
    c.r = 255.0f;
    c.g = 255.0f;
    c.b = 255.0f;
    c.a = 255.0f;
    func_08873DB0(&c, 2);
    s->state = state;
}
