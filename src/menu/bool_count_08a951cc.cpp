struct A6CountRec {
    char pad_00[8];
    int field_08;
};

extern "C" {
bool func_08A951CC(int unused, char *base) {
    A6CountRec *r = (A6CountRec *)(base + 0x240);
    if (r->field_08 > 1) {
        return true;
    }
    return false;
}
}
