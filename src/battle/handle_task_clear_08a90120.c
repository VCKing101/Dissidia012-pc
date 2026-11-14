typedef struct HandleTaskState {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    int handle;
    int field_0C;
    int field_10;
    char pad_14[0x4];
    int field_18;
    float field_1C;
} HandleTaskState;

void func_08A90120(HandleTaskState *self)
{
    self->bound = 0;
    self->handle = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_18 = 0;
    self->field_1C = -1.0f;
}
