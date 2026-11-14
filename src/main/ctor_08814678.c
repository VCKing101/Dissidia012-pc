typedef struct Agent4Node14678 {
    unsigned char base[0x2C];
    void *vtable;
    int owner;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
} Agent4Node14678;

extern int D_089C6FE8;
void func_088A985C(Agent4Node14678 *self);
void func_08935828(void *dst, int value, int size);

Agent4Node14678 *func_08814678(Agent4Node14678 *self, int owner, int a, int b, int c) {
    func_088A985C(self);
    self->vtable = &D_089C6FE8;
    self->owner = owner;
    func_08935828(&self->field_34, 0, 0x14);
    self->field_34 = a;
    self->field_38 = b;
    self->field_3C = c;
    self->field_40 = 0;
    self->field_44 = 0;
    return self;
}
