/* Clears every active slot of the second table whose id matches. */
struct Slot08806358 {
    unsigned char active;
    unsigned char field_01;
    unsigned char id;
    unsigned char field_03;
    float value;
};

struct Owner08806358 {
    char pad_000[0x604];
    Slot08806358 slots[16];
};

extern "C" {

void func_08806358(Owner08806358 *self, unsigned int id)
{
    for (int i = 0; i < 16; i++) {
        Slot08806358 *slot = &self->slots[i];
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
