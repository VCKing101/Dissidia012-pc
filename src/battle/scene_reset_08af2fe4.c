typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct SceneReset {
    char pad_00[0x4];
    char field_04[0x14];
    char field_18[0x44];
    int field_5C;
    char pad_60[0x10];
    unsigned char field_70;
    unsigned char field_71;
    unsigned char field_72;
    unsigned char field_73;
} SceneReset;

extern int func_088D4C5C(void);
extern void func_08873D34(int a, int b, int c, int d, int e, float time);
extern void func_08873DB0(Vec4 *color, int mode);
extern void func_088EC634(void *sub);
extern void func_088EC34C(void *sub);

void func_08AF2FE4(SceneReset *self)
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
    self->field_70 = 0;
    self->field_71 = 0;
    self->field_73 = 0;
    self->field_5C = 1;
}
