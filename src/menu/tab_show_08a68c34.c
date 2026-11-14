/* Highlights the tab for the current mode (0, 1 or 2) and dims the other two. */
typedef struct ShowTabs {
    char pad_00[0x28];
    int field_28;
    char pad_2C[0x44 - 0x2C];
    void *field_44;
    char pad_48[0x54 - 0x48];
    void *field_54;
    char pad_58[0x6C - 0x58];
    void *field_6C;
} ShowTabs;

extern void func_088D9CD8(void *icon);
extern void func_088D9CFC(void *icon);

void func_08A68C34(ShowTabs *self)
{
    switch (self->field_28) {
    case 0:
        func_088D9CD8(self->field_44);
        func_088D9CFC(self->field_54);
        func_088D9CFC(self->field_6C);
        break;
    case 1:
        func_088D9CFC(self->field_44);
        func_088D9CD8(self->field_54);
        func_088D9CFC(self->field_6C);
        break;
    case 2:
        func_088D9CFC(self->field_44);
        func_088D9CFC(self->field_54);
        func_088D9CD8(self->field_6C);
        break;
    }
}
