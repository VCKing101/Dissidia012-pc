typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct HandleTaskState {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    int handle;
    unsigned char field_0C;
    char pad_0D[0x3];
    int field_10;
    float field_14;
    int field_18;
    char pad_1C[0x4];
    Vec4 field_20;
    float field_30;
} HandleTaskState;

extern void func_089A3890(Vec4 *v);

void func_08A8F850(HandleTaskState *self)
{
    self->bound = 0;
    self->handle = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0.0f;
    self->field_18 = 0;
    func_089A3890(&self->field_20);
    self->field_30 = 0.0f;
}
