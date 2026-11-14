/* Constructor: vtable, screen singleton, self link, cleared part table;
   then loads the layout. */
/* The object's first word, as the base part sees it. */
struct Base08B2E270 {
    int field_000;
};

struct Screen08B2E270 {
    int field_000;
    int field_004;
    char pad_008[0x1C - 8];
    char field_01C[0x49C - 0x1C];
    void *field_49C[1];
    unsigned short field_4A0[1];
    char pad_4A2[0x844 - 0x4A2];
    struct Screen08B2E270 *field_844;
    void *vtable;
};

extern int D_08B2F500;
extern struct Screen08B2E270 *D_08B2F640;
extern void func_088D3B5C(char *layout, int size);

struct Screen08B2E270 *func_08B2E270(struct Screen08B2E270 *self)
{
    int i;
    self->vtable = &D_08B2F500;
    D_08B2F640 = self;
    self->field_844 = self;
    ((struct Base08B2E270 *)self)->field_000 = 0;
    self->field_844->field_004 = 0;
    for (i = 0; i < 1; i++) {
        self->field_844->field_49C[i] = 0;
        self->field_844->field_4A0[i] = 0xFFFF;
    }
    func_088D3B5C(self->field_844->field_01C, 0x120);
    return self;
}
