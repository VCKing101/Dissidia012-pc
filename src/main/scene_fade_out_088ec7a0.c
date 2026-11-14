typedef struct Color { float r; float g; float b; float a; } Color;
typedef struct Scene { int field_00; char menu[0x14]; char list[0xC]; unsigned char open; } Scene;
extern int func_088D4C5C(void);
extern void func_08873D34(float time, int mode, void (*done)(Scene *s), Scene *s, int a, int b);
extern void func_08873DB0(Color *c, int mode);
extern void func_088EC634(void *menu);
extern void func_088EC34C(void *list);

void func_088EC7A0(Scene *s)
{
    Color c;
    func_08873D34((float)func_088D4C5C(), 2, 0, 0, 0, 2);
    c.r = 0.0f;
    c.g = 0.0f;
    c.b = 0.0f;
    c.a = 0.0f;
    func_08873DB0(&c, 2);
    func_088EC634(s->menu);
    func_088EC34C(s->list);
    s->open = 0;
}
