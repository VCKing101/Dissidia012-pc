typedef struct HandleTaskState {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    int handle;
    int field_0C;
    float field_10;
    float field_14;
    int field_18;
} HandleTaskState;

void func_08A89880(HandleTaskState *self)
{
    self->bound = 0;
    self->handle = 0;
    self->field_0C = -1;
    self->field_10 = 0.0f;
    self->field_14 = 0.0f;
    self->field_18 = 0;
}
