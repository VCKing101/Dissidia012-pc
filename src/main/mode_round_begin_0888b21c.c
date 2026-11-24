/* Begins a mode round: resets the battle systems and both units' state,
   resets the camera to its start position and resumes the pause menu in
   mode 5. */
typedef struct ModeUnit {
    char pad_000[0x188];
    unsigned int flags;
    char pad_18C[0x784 - 0x18C];
    char status[0xBA0 - 0x784];
    char motion[4];
} ModeUnit;

typedef struct ModeCtl {
    char pad_00[0x38];
    ModeUnit *units[4];
    char pad_48[4];
    int kind;
} ModeCtl;

typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct Camera {
    char pad_00[0x50];
    Vec4 a;
    Vec4 b;
    Vec4 c;
} Camera;

extern int D_08ACF110;
extern int D_08ACA350;
extern int D_08B04BA0;
extern int D_08AB6738;
extern int D_08ACEE24;
extern int D_08A405E0;
extern int D_08A50568;
extern int D_089D6AE8;
extern int D_08A62280;
extern int D_08B13B48;
extern Camera D_09B49140;
extern Vec4 D_09AF84C0;

extern void func_08ACEF94(int p);
extern void func_08AC9034(int p);
extern void func_08B01870(int p);
extern void func_08AB54E4(int p);
extern void func_08AC6080(void *motion);
extern void func_08ACA8B4(void *status);
extern void func_08ACDC88(int p);
extern void func_08A3E844(int p);
extern void func_08A4FFD8(int p);
extern void func_0888A19C(void);
extern void func_08895FE4(int p);
extern void func_088885DC(ModeCtl *self);
extern void func_08A62120(int p);
extern void func_08AEEF4C(Camera *cam, Vec4 *pos, int flag);
extern void func_089A3868(Vec4 *dst, Vec4 *src);
extern void func_08B13314(int menu);

void func_0888B21C(ModeCtl *self)
{
    int i;
    Vec4 v;
    Camera *cam;

    func_08ACEF94(D_08ACF110);
    func_08AC9034(D_08ACA350);
    func_08B01870(D_08B04BA0);
    func_08AB54E4(D_08AB6738);
    for (i = 0; i < 2; i++) {
        if (self->units[i] != 0) {
            func_08AC6080(self->units[i]->motion);
            func_08ACA8B4(self->units[i]->status);
            self->units[i]->flags &= ~8;
        }
    }
    func_08ACDC88(D_08ACEE24);
    func_08A3E844(D_08A405E0);
    func_08A4FFD8(D_08A50568);
    func_08AB54E4(D_08AB6738);
    func_0888A19C();
    func_08895FE4(D_089D6AE8);
    func_088885DC(self);
    func_08A62120(D_08A62280);
    cam = &D_09B49140;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    v.w = 0.0f;
    func_08AEEF4C(cam, &v, 0);
    func_089A3868(&cam->c, &D_09AF84C0);
    func_089A3868(&cam->b, &cam->c);
    func_089A3868(&cam->a, &cam->b);
    if (self->kind == 5) {
        func_08B13314(D_08B13B48);
    }
}
