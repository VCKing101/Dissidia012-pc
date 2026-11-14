/* Updates the owner's three 12-byte slots. */
struct Slot0884246C {
    int field_00;
    int field_04;
    int field_08;
};

struct Owner0884246C {
    Slot0884246C slots[3];
};

extern "C" {

void func_088427F4(Owner0884246C *owner, Slot0884246C *slot);

void func_0884246C(Owner0884246C *owner)
{
    for (int i = 0; i < 3; i++) {
        Slot0884246C *slot = &owner->slots[i];
        func_088427F4(owner, slot);
    }
}

}
