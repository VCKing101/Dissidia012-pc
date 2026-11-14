/* Releases the eight held objects of a slot table, then clears its count. */
struct SlotTable {
    int field_00;
    void *slots[8];
    int field_24;
};

extern "C" {

void func_08808214(void *obj);

void func_0883E1F0(SlotTable *self)
{
    for (int i = 0; i < 8; i++) {
        if (self->slots[i] != 0) {
            func_08808214(self->slots[i]);
            self->slots[i] = 0;
        }
    }
    self->field_24 = 0;
}

}
