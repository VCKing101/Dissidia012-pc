typedef struct Color { float r; float g; float b; float a; } Color;
typedef struct Scene { int field_00; char menu[0x14]; } Scene;
extern int func_088D4C5C(void);
extern void func_08873D34(float time, int mode, void (*done)(Scene *s), Scene *s, int a, int b);
extern void func_08873DB0(Color *c, int mode);
extern void func_088EC658(void *menu);
extern void func_088EC8CC(Scene *s);

void func_088EC818(Scene *s)
{
    Color c;
    func_08873D34((float)func_088D4C5C(), 3, func_088EC8CC, s, 0, 2);
    c.r = 255.0f;
    c.g = 255.0f;
    c.b = 255.0f;
    c.a = 255.0f;
    func_08873DB0(&c, 2);
    func_088EC658(s->menu);
}
