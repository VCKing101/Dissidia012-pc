/* Clears every active slot of the first table whose id matches. */
struct Slot088062CC {
    unsigned char active;
    unsigned char field_01;
    unsigned char id;
    unsigned char field_03;
    float value;
};

struct Owner088062CC {
    char pad_000[0x584];
    Slot088062CC slots[16];
};

extern "C" {

void func_088062CC(Owner088062CC *self, unsigned int id)
{
    for (int i = 0; i < 16; i++) {
        Slot088062CC *slot = &self->slots[i];
        if (slot->active != 0 && slot->id == id) {
            slot->active = 0;
            slot->field_01 = 0;
            slot->id = 0;
            slot->field_03 = 0;
            slot->value = 0.0f;
        }
    }
}

}
