struct EnableSlot {
    char pad_00[0x12];
    short count;
    char pad_14[7];
    bool active;
};

extern "C" {

void func_0884EDD0(EnableSlot *self)
{
    self->count = 1;
    self->active = true;
}

}
