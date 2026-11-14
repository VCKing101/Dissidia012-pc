/* Whether member slot `index` (0-4) holds a character that is not in state 0 or 2. */
struct UsableSlot {
    short field_00;
    short field_02;
    char pad_04[0x2DC - 0x4];
};

struct UsableParty {
    char pad_000[0x66C];
    UsableSlot *field_66C;
};

extern "C" {
bool func_08A6AF70(UsableParty *self, int index)
{
    if (index < 0 || 4 < index) {
        return false;
    }
    UsableSlot *slot = &self->field_66C[index];
    switch (slot->field_02) {
    case 0:
    case 2:
        return false;
    }
    if (slot->field_00 == 0) {
        return false;
    }
    return true;
}
}
