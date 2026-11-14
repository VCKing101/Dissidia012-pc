/* Sets list icon `index` to `value`, clears its flag 2 and refreshes it. */
typedef struct ListIcon {
    char pad_00[0x20];
    unsigned short field_20;
} ListIcon;

typedef struct ListIcons {
    int field_00;
    ListIcon **field_04;
} ListIcons;

typedef struct ListIconOwner {
    char pad_0000[0x2A14];
    ListIcons *field_2A14;
} ListIconOwner;

extern void func_088D9AEC(float value, ListIcon *icon);
extern void func_088D9CD8(ListIcon *icon);

void func_08A3D420(ListIconOwner *self, int index, int value)
{
    func_088D9AEC((float)value, self->field_2A14->field_04[index]);
    self->field_2A14->field_04[index]->field_20 = self->field_2A14->field_04[index]->field_20 & ~2;
    func_088D9CD8(self->field_2A14->field_04[index]);
}
