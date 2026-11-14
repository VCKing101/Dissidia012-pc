typedef struct SceneRange {
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;

    static void init(SceneRange *range)
    {
        range->field_5C = 0;
        range->field_60 = 0;
        range->field_64 = 4;
        range->field_68 = 0;
        range->field_6C = -1;
    }
} SceneRange;

typedef struct SceneObject {
    const void *vtable;
    char field_04[0x14];
    char field_18[0xC];
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    unsigned char field_3C;
    char pad_3D[0x1F];
    SceneRange range;
    unsigned char field_70;
    unsigned char field_71;
    unsigned char field_72;
    unsigned char field_73;
    char pad_74[0x4];
    int field_78;
    const void *listener_vtable;
    struct SceneObject *listener_owner;
    int field_84;
    char field_88[0x10];
    int field_98;
} SceneObject;

extern "C" {

extern const char D_08AFB520[];
extern const char D_08AFB4E0[];
extern void func_088EC4C4(void *sub);
extern void func_088EC278(void *sub);
extern void func_0887B1E4(void *sub);

SceneObject *func_08AF2C10(SceneObject *self)
{
    self->vtable = D_08AFB520;
    func_088EC4C4(self->field_04);
    func_088EC278(self->field_18);
    self->field_24 = 0;
    self->field_28 = 0;
    self->field_2C = 0;
    self->field_30 = 0;
    self->field_34 = 0;
    self->field_38 = 0;
    SceneRange::init(&self->range);
    self->field_70 = 0;
    self->field_71 = 0;
    self->field_72 = 0;
    self->field_73 = 0;
    self->field_78 = 0;
    self->listener_vtable = D_08AFB4E0;
    self->listener_owner = self;
    self->field_84 = 0;
    func_0887B1E4(self->field_88);
    self->field_98 = 0;
    self->field_3C = 0;
    return self;
}

}
