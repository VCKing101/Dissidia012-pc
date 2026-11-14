/* 0x20-byte slots: a value is only reported while the slot is in use and active. */
struct ValueSlot {
    char pad_00[8];
    int data;
    int field_0C;
    short value;
    short count;
    char pad_14[7];
    unsigned char active;
    char pad_1C[4];

    static bool empty(ValueSlot *s) { return s->count == 0; }
};

extern "C" {

int func_0884F67C(ValueSlot *slots, int i)
{
    ValueSlot *s = &slots[i];
    if (ValueSlot::empty(s) || s->active == 0) {
        return 0;
    } else {
        return s->value;
    }
}

int func_0884F6B8(ValueSlot *slots, int i)
{
    ValueSlot *s = &slots[i];
    if (ValueSlot::empty(s) || s->active == 0) {
        return 0;
    } else {
        return s->data;
    }
}

}
