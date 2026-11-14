/* Draws the frame and its two cursor layers (the alternate pair when flag
   0x20 is set), unless `skip` is set. */
struct Frame08AF5030 {
    void *field_00;
    char *field_04;
    char *field_08;
    char pad_0C[0x20 - 0xC];
    unsigned char field_20;
};

struct State08AF5030 {
    char pad_0000[0x2C40];
    Frame08AF5030 field_2C40;
};

struct Owner08AF5030 {
    char pad_000[0x140];
    State08AF5030 *field_140;
};

extern "C" {
extern unsigned short D_08B0067E[];
extern unsigned short D_08B006BE[];
extern unsigned short D_08B00726[];
extern unsigned short D_08B007EE[];
extern unsigned short D_08B0085E[];
void func_088D3EA0(void *owner, char *layout, unsigned short *table);
void func_088D442C(char *layout, char *cursor, unsigned short *table);
}

struct Draw08AF5030 {
    static void layer(char *cursor, char *layout, unsigned short *table) { func_088D442C(layout, cursor, table); }
};

extern "C" {
void func_08AF5030(Owner08AF5030 *self, int skip)
{
    if (skip == 0) {
        Frame08AF5030 *frame = &self->field_140->field_2C40;
        func_088D3EA0(frame->field_00, frame->field_04, D_08B0067E);
        if (frame->field_20 == 0) {
            Draw08AF5030::layer(frame->field_08, frame->field_04, D_08B006BE);
            func_088D442C(frame->field_04, frame->field_08, D_08B00726);
        } else {
            Draw08AF5030::layer(frame->field_08, frame->field_04, D_08B007EE);
            func_088D442C(frame->field_04, frame->field_08, D_08B0085E);
        }
    }
}
}
