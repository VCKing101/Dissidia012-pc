/* Draws the slot texts: all of them (mode != 0), the full list (id -1), or
   the one entry whose id matches. */

typedef struct Text08B03584 {
    unsigned short id;
    unsigned short field_2;
    unsigned short field_4;
    unsigned short field_6;
} Text08B03584;

typedef struct Layout08B03584 {
    char pad_000[0x400];
    char field_400[0x5D4 - 0x400];
    char field_5D4[4];
} Layout08B03584;

typedef struct Owner08B03584 {
    char pad_000[0x67C];
    Layout08B03584 *field_67C;
} Owner08B03584;

extern Text08B03584 *func_08B0461C(void);
extern void func_088D436C(char *layout, char *cursor, Text08B03584 *texts);
extern void func_088D428C(char *layout, char *cursor, Text08B03584 *texts, int show);
extern void func_088D40BC(char *layout, char *cursor, int id, int a, int b, int c);

void func_08B03584(Owner08B03584 *self, int mode, unsigned int id)
{
    Text08B03584 *text = func_08B0461C();
    int i;
    if (mode == 0) {
        if (id != 0xFFFFFFFF) {
            for (i = 0; i < 0x19; i++) {
                if (text[i].id == id) {
                    func_088D40BC(self->field_67C->field_400, self->field_67C->field_5D4, text[i].id,
                                  text[i].field_2, text[i].field_4, text[i].field_6);
                    break;
                }
            }
        } else {
            func_088D428C(self->field_67C->field_400, self->field_67C->field_5D4, text, 1);
        }
    } else {
        func_088D436C(self->field_67C->field_400, self->field_67C->field_5D4, text);
    }
}
