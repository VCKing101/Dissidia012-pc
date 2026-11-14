struct Agent4ZeroPair {
    int field_00;
    int field_04;

    static void clear(Agent4ZeroPair *pair) {
        pair->field_04 = 0;
        pair->field_00 = 0;
    }
};

struct Agent4Range {
    short field_00;
    short field_02;
    int field_04;
    Agent4ZeroPair first;
    Agent4ZeroPair second;
};

extern "C" {
void func_0881F47C(Agent4Range *self) {
    self->field_00 = 0;
    self->field_02 = 0;
    self->field_04 = 0;
    Agent4ZeroPair::clear(&self->first);
    Agent4ZeroPair::clear(&self->second);
}
}
