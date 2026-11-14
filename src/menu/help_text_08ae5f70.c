/* Sends the help text id for the current cursor mode (0xE when neither mode
   2 nor mode 4 is set). */
struct State08AE5F70 {
    char pad_0000[0x2558];
    unsigned int field_2558;
    char pad_255C[0x95C4 - 0x255C];
    short field_95C4;
};

struct Screen08AE5F70 {
    char pad_0000[0xA690];
    struct State08AE5F70 *field_A690;
};

extern void *D_08B01CF8;
extern int func_08AF96B0(void);
extern void func_08AF6E10(void *queue, int text);

void func_08AE5F70(struct Screen08AE5F70 *self)
{
    int text = 0xE;
    if ((self->field_A690->field_2558 & 4) != 0) {
        text = (self->field_A690->field_95C4 == 0) ? 0x13 : 0x14;
    } else if ((self->field_A690->field_2558 & 2) != 0) {
        if (self->field_A690->field_95C4 == 0) {
            text = (func_08AF96B0() != 0) ? 0x10 : 0xF;
        } else {
            text = (func_08AF96B0() != 0) ? 0x12 : 0x11;
        }
    }
    func_08AF6E10(D_08B01CF8, text);
}
