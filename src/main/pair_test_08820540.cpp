struct Agent4ShortPair {
    short field_00;
    short field_02;

    static bool has_first(Agent4ShortPair *pair) {
        return pair->field_00 != 0;
    }
};

extern "C" {
bool func_08820540(Agent4ShortPair *self) {
    if (!Agent4ShortPair::has_first(self)) {
        return false;
    }
    return self->field_02 != 0;
}
}
