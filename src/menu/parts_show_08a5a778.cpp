/* Resets all eight parts (virtual slot 3), then shows the selected part, or the empty label when none is selected. */
class GridPart {
    char data[0x8];
public:
    virtual void slot_08();
    virtual void slot_10();
    virtual void slot_18();
};

struct GridOwner {
    char pad_00[0x4C];
    void *field_4C;
    char pad_50[0x8];
    GridPart *field_58[8];
    char pad_78[0x19E0 - 0x78];
    int field_19E0;
};

extern "C" {
void func_088D9CFC(void *label);
void func_088D9CD8(void *label);

void func_08A5A778(GridOwner *self)
{
    int selected = self->field_19E0;
    int i;
    for (i = 0; i < 8; i++) {
        self->field_58[i]->slot_18();
    }
    func_088D9CFC(self->field_4C);
    if (selected < 8) {
        self->field_58[selected]->slot_10();
    } else {
        func_088D9CD8(self->field_4C);
    }
}
}
