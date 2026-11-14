typedef struct SceneState {
    char pad_000[0x418];
    int field_418;
    int field_41C;
    int field_420;
    int field_424;
    int field_428;
    int field_42C;
    int field_430;
} SceneState;

typedef struct SceneOwner {
    char pad_000[0x154];
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170[5];
    char pad_184[0x30C - 0x184];
    SceneState *field_30C;
} SceneOwner;

typedef struct FadeColor {
    float r;
    float g;
    float b;
    float a;
} FadeColor;

extern int func_088D4C5C(void);
extern void func_08873D34(int layer, void (*done)(void), int a, int b, int c, float frames);
extern void func_08873DB0(FadeColor *color, int layer);
extern void func_08A8A1FC(void);

void func_08A890EC(SceneOwner *self)
{
    FadeColor color;
    int i;
    int frames;
    self->field_30C->field_418 = 1;
    self->field_154 = 0;
    self->field_158 = 0;
    self->field_30C->field_41C = 0;
    self->field_30C->field_420 = 0;
    self->field_30C->field_428 = 0;
    self->field_30C->field_424 = -1;
    self->field_30C->field_42C = -1;
    self->field_30C->field_430 = 1;
    self->field_15C = 0;
    self->field_160 = 0;
    self->field_164 = 0;
    self->field_168 = 0;
    self->field_16C = 0;
    for (i = 0; i < 5; i++) {
        self->field_170[i] = 0;
    }
    frames = func_088D4C5C();
    func_08873D34(2, func_08A8A1FC, 0, 1, 2, (float)frames);
    color.r = 0.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 0.0f;
    func_08873DB0(&color, 2);
}
