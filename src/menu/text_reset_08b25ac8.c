/* Resets the offsets (0x30..0x38) of the part of every text entry `id`. */
typedef struct Text08B25AC8 {
    unsigned short id;
    unsigned short field_2;
    unsigned short field_4;
    unsigned short field_6;
} Text08B25AC8;

typedef struct Part08B25AC8 {
    char pad_00[0x30];
    int field_30;
    int field_34;
    int field_38;
} Part08B25AC8;

typedef struct State08B25AC8 {
    char pad_0[4];
    Part08B25AC8 *field_4[1];
} State08B25AC8;

typedef struct Owner08B25AC8 {
    State08B25AC8 *field_0;
} Owner08B25AC8;

extern Text08B25AC8 *func_08B28000(void);

void func_08B25AC8(Owner08B25AC8 *self, unsigned int id)
{
    Text08B25AC8 *text = func_08B28000();
    int i;
    for (i = 0; i < 0x2E; i++) {
        if (text[i].id == id) {
            Part08B25AC8 *part = self->field_0->field_4[text[i].field_4];
            part->field_34 = 0;
            part->field_30 = 0;
            part->field_38 = 0;
        }
    }
}
