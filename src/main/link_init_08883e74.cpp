extern "C" float D_089C9F50;

struct Link08883E74 {
    unsigned char field_00;
    unsigned char unknown_01[3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    float field_18;
    int field_1C;
    int field_20;
    unsigned char field_24;
    unsigned char field_25;

    static Link08883E74 *init(Link08883E74 *self) {
        self->field_00 = 0;
        self->field_04 = 1;
        self->field_08 = 0;
        self->field_0C = 0;
        self->field_10 = 2;
        self->field_14 = 0;
        self->field_18 = D_089C9F50;
        self->field_1C = 0;
        self->field_20 = 0;
        self->field_24 = 0;
        self->field_25 = 0;
        self->field_04 = 0;
        return self;
    }
};

extern "C" {
Link08883E74 *func_08883E74(Link08883E74 *self) {
    return Link08883E74::init(self);
}
}
