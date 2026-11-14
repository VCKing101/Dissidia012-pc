/* Draws the window frame and title, then every row's label and value, and
   refreshes the cursor. */
struct Label08ACB950 {
    char data[0x30];
};

struct Value08ACB950 {
    char data[0x38];
};

struct State08ACB950 {
    char pad_0000[0x400];
    char field_400[0x880 - 0x400];
    char field_880[0x121C - 0x880];
    int field_121C;
    char pad_1220[0xBEC4 - 0x1220];
    unsigned short field_BEC4[0x28];
    struct Label08ACB950 field_BF14[9];
    struct Value08ACB950 field_C0C4[1];
};

struct Screen08ACB950 {
    char pad_0000[0xDB30];
    struct State08ACB950 *field_DB30;
};

extern unsigned short D_08AD993C[];
extern void func_088D3EA0(struct State08ACB950 *state, char *layout, void *text);
extern void func_088D442C(char *layout, char *cursor, void *value);
extern void func_08ACD184(struct Screen08ACB950 *self);

void func_08ACB950(struct Screen08ACB950 *self)
{
    int i;
    func_088D3EA0(self->field_DB30, self->field_DB30->field_400, D_08AD993C);
    func_088D3EA0(self->field_DB30, self->field_DB30->field_400, self->field_DB30->field_BEC4);
    for (i = 0; i < self->field_DB30->field_121C; i++) {
        func_088D3EA0(self->field_DB30, self->field_DB30->field_400, &self->field_DB30->field_BF14[i]);
        func_088D442C(self->field_DB30->field_400, self->field_DB30->field_880, &self->field_DB30->field_C0C4[i]);
    }
    func_08ACD184(self);
}
