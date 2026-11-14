extern "C" {
void func_08895430(void *self);
extern char D_089CA8B0[];
}

struct View08895528 {
    int field_00;
    int field_04;
    void *vtable;
    int field_0C;
    int field_10;
    int width;
    int height;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    unsigned char field_44;
    unsigned char field_45;

    static View08895528 *construct(View08895528 *self, void *vt) {
        func_08895430(self);
        self->vtable = vt;
        self->field_0C = 0;
        self->field_10 = 0;
        self->width = 0x1E0;
        self->height = 0x110;
        self->field_1C = 0;
        self->field_20 = 0;
        self->field_24 = 0;
        self->field_28 = 0;
        self->field_2C = 0;
        self->field_30 = 0;
        self->field_34 = 2;
        self->field_38 = 3;
        self->field_3C = 0;
        self->field_40 = 0;
        self->field_44 = 1;
        self->field_45 = 1;
        return self;
    }
};

extern "C" {
View08895528 *func_08895528(View08895528 *self) {
    return View08895528::construct(self, D_089CA8B0);
}
}
