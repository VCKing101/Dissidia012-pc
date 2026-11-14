/* Shows the label of every list (of five) that has entries and hides the others. */
typedef struct ListLabel {
    char pad_00[0x20];
    unsigned short field_20;
} ListLabel;

typedef struct ListLabels {
    char pad_00[0xC];
    ListLabel *field_0C[5];
} ListLabels;

typedef struct ListState {
    char pad_0000[0x400];
    ListLabels field_400;
    char pad_0420[0x1524 - 0x420];
    char field_1524[4];
} ListState;

typedef struct ListOwner {
    char pad_000[0x138];
    ListState *field_138;
} ListOwner;

extern int func_08A298F0(int list, char *rows);
extern void func_088D9D24(ListLabel *label);

void func_08A2BA98(ListOwner *self)
{
    ListLabels *labels = &self->field_138->field_400;
    int i;
    for (i = 0; i < 5; i++) {
        if (func_08A298F0(i, self->field_138->field_1524) != 0) {
            ListLabel *label = labels->field_0C[i];
            label->field_20 &= ~0x4000;
            func_088D9D24(label);
        } else {
            ListLabel *label = labels->field_0C[i];
            label->field_20 |= 0x4000;
            func_088D9D24(label);
        }
    }
}
