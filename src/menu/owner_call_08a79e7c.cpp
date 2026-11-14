/* Passes the value to every used slot's object (virtual slot 3), from the last slot down. */
class SlotObject {
public:
    virtual void slot_08();
    virtual void slot_10();
    virtual void slot_18(void *value);
};

struct ObjectSlot {
    int field_00;
    SlotObject *field_04;
    char pad_08[0x8];
};

struct ObjectSlots {
    ObjectSlot field_00[8];
    char pad_80[0x4];
    int field_84;
};

extern "C" {
void func_08A79E7C(ObjectSlots *self, void *value)
{
    for (int i = self->field_84; i >= 0; i--) {
        SlotObject *object = self->field_00[i].field_04;
        if (object != 0) {
            object->slot_18(value);
        }
    }
}
}
