/* While paging is active (mode 1), shows icon 7 and steps to the next page,
   or ends paging (mode 2) on the last one; then redraws. Returns 1 when it
   acted. */
struct State08AD0364 {
    char pad_0000[0x77C];
    void *field_77C;
    char pad_0780[0xCAC0 - 0x780];
    unsigned int field_CAC0;
    int field_CAC4;
    int field_CAC8;
    char pad_CACC[0xCAD4 - 0xCACC];
    int field_CAD4;
    int field_CAD8;
};

struct Screen08AD0364 {
    char pad_0000[0xDB30];
    struct State08AD0364 *field_DB30;
};

extern void func_088D9F5C(void *icon, int id, int show);
extern void func_08AD030C(struct Screen08AD0364 *self);
extern void func_08AD07F0(struct Screen08AD0364 *self);

int func_08AD0364(struct Screen08AD0364 *self)
{
    int acted = 0;
    if (self->field_DB30->field_CAC4 != 0 && self->field_DB30->field_CAC8 == 1) {
        func_088D9F5C(self->field_DB30->field_77C, 7, 1);
        if (self->field_DB30->field_CAD4 >= self->field_DB30->field_CAD8 - 1) {
            self->field_DB30->field_CAC8 = 2;
        } else {
            self->field_DB30->field_CAD4 = self->field_DB30->field_CAD4 + 1;
            func_08AD030C(self);
        }
        acted = 1;
    }
    if (acted != 0) {
        func_08AD07F0(self);
        self->field_DB30->field_CAC0 |= 2;
    }
    return acted;
}
