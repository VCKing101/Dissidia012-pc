struct Agent4ZeroPair {
    int field_00;
    int field_04;

    static void clear(Agent4ZeroPair *pair) {
        pair->field_04 = 0;
        pair->field_00 = 0;
    }
};

extern "C" {
extern int D_089C7530;
void func_08816744(void *part);
void func_0881A1CC(void *part);
void func_08819B8C(void *part);
void func_0881B4A0(struct Agent4Manager1B39C *self);
}

struct Agent4Manager1B39C {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14[0x20];
    unsigned char field_34[0x28];
    unsigned char field_5C[0xB4];
    Agent4ZeroPair pairs[6];
    unsigned char unknown_0140[8];
    void *vtable;

    static Agent4Manager1B39C *construct(Agent4Manager1B39C *self, void *vtable) {
        self->vtable = vtable;
        self->field_0C = 1;
        func_08816744(self->field_14);
        func_0881A1CC(self->field_34);
        func_08819B8C(self->field_5C);
        Agent4ZeroPair::clear(&self->pairs[0]);
        Agent4ZeroPair::clear(&self->pairs[1]);
        Agent4ZeroPair::clear(&self->pairs[2]);
        Agent4ZeroPair::clear(&self->pairs[3]);
        Agent4ZeroPair::clear(&self->pairs[4]);
        Agent4ZeroPair::clear(&self->pairs[5]);
        func_0881B4A0(self);
        return self;
    }
};

extern "C" {
/* Constructor; vtable 0x089C7530. */
Agent4Manager1B39C *func_0881B39C(Agent4Manager1B39C *self) {
    return Agent4Manager1B39C::construct(self, &D_089C7530);
}
}
