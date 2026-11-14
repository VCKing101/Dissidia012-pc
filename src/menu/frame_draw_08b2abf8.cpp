/* Draws the window frame, then the full or the short layout depending on
   func_08B2A9B8, then the common cursor layer; nothing in mode 4. */
struct State08B2ABF8 {
    char pad_000[0xC];
    int field_00C;
    char pad_010[0x18 - 0x10];
    void *field_018;
    char field_01C[0x49C - 0x1C];
    char field_49C[4];
};

struct Owner08B2ABF8 {
    char pad_000[0xA18];
    State08B2ABF8 *field_A18;
};

extern "C" {
extern unsigned short D_08B2CCA0[];
extern unsigned short D_08B2D064[];
extern unsigned short D_08B2D09C[];
extern unsigned short D_08B2D0AC[];
extern unsigned short D_08B2D0BC[];
extern unsigned short D_08B2D02C[];
void func_088D3F00(void *owner, char *layout, unsigned short *table);
void func_088D44EC(char *layout, char *cursor, unsigned short *table);
int func_08B2A9B8(Owner08B2ABF8 *self);
}

struct Draw08B2ABF8 {
    static void frame(char *layout, void *owner, unsigned short *table) { func_088D3F00(owner, layout, table); }
};

extern "C" {
void func_08B2ABF8(Owner08B2ABF8 *self)
{
    if (self->field_A18->field_00C == 4) {
        return;
    }
    func_088D3F00(self->field_A18->field_018, self->field_A18->field_01C, D_08B2CCA0);
    if (func_08B2A9B8(self) != 0) {
        Draw08B2ABF8::frame(self->field_A18->field_01C, self->field_A18->field_018, D_08B2D064);
        func_088D44EC(self->field_A18->field_01C, self->field_A18->field_49C, D_08B2D09C);
    } else {
        Draw08B2ABF8::frame(self->field_A18->field_01C, self->field_A18->field_018, D_08B2D0AC);
        func_088D44EC(self->field_A18->field_01C, self->field_A18->field_49C, D_08B2D0BC);
    }
    func_088D44EC(self->field_A18->field_01C, self->field_A18->field_49C, D_08B2D02C);
}
}
