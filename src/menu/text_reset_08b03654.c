/* Resets the offsets (0x30..0x38) of the part of every text entry `id`. */
typedef struct Text08B03654 {
    unsigned short id;
    unsigned short field_2;
    unsigned short field_4;
    unsigned short field_6;
} Text08B03654;

typedef struct Part08B03654 {
    char pad_00[0x30];
    int field_30;
    int field_34;
    int field_38;
} Part08B03654;

typedef struct Layout08B03654 {
    char pad_000[0x400];
    Part08B03654 *field_400[1];
} Layout08B03654;

typedef struct Owner08B03654 {
    char pad_000[0x67C];
    Layout08B03654 *field_67C;
} Owner08B03654;

extern Text08B03654 *func_08B0461C(void);

void func_08B03654(Owner08B03654 *self, unsigned int id)
{
    Text08B03654 *text = func_08B0461C();
    int i;
    for (i = 0; i < 0x19; i++) {
        if (text[i].id == id) {
            Part08B03654 *part = self->field_67C->field_400[text[i].field_4];
            part->field_34 = 0;
            part->field_30 = 0;
            part->field_38 = 0;
        }
    }
}
