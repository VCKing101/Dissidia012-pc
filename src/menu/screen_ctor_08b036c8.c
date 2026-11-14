/* Constructor: base init, vtable, body pointer and three cleared fields. */
typedef struct Body08B036C8 {
    char pad_000[0x638];
    int field_638;
    int field_63C;
    int field_640;
} Body08B036C8;

typedef struct Screen08B036C8 {
    void *vtable;
    char field_004[0x67C - 4];
    Body08B036C8 *field_67C;
} Screen08B036C8;

extern int D_08B044F8;
extern void func_08AF1230(Screen08B036C8 *self);

Screen08B036C8 *func_08B036C8(Screen08B036C8 *self)
{
    Body08B036C8 *body;
    func_08AF1230(self);
    self->vtable = &D_08B044F8;
    body = (Body08B036C8 *)self->field_004;
    self->field_67C = body;
    body->field_638 = 0;
    self->field_67C->field_63C = 0;
    self->field_67C->field_640 = 0;
    return self;
}
