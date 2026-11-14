typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct ScenePanelReset {
    char pad_00[0x4];
    char field_04[0x14];
    char field_18[0x10];
    int field_28;
    unsigned char field_2C;
} ScenePanelReset;

extern int func_088D4C5C(void);
extern void func_08873D34(int a, int b, int c, int d, int e, float time);
extern void func_08873DB0(Vec4 *color, int mode);
extern void func_088EC634(void *sub);
extern void func_088EC34C(void *sub);

void func_08AFC010(ScenePanelReset *self)
{
    Vec4 color;
    func_08873D34(2, 0, 0, 0, 2, (float)func_088D4C5C());
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    color.w = 0.0f;
    func_08873DB0(&color, 2);
    func_088EC634(self->field_04);
    func_088EC34C(self->field_18);
    self->field_2C = 0;
    self->field_28 = 1;
}
