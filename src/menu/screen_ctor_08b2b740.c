/* Constructor: vtable, screen singleton, self link, cleared part tables;
   then loads the layout and builds the screen. */
/* The object's first word, as the base part sees it. */
struct Base08B2B740 {
    int field_000;
};

struct Screen08B2B740 {
    int field_000;
    int field_004;
    char pad_008[0x1C - 8];
    char field_01C[0x49C - 0x1C];
    void *field_49C[0x51];
    unsigned short field_5E0[0x51];
    char pad_682[0xA18 - 0x682];
    struct Screen08B2B740 *field_A18;
    void *vtable;
};

extern int D_08B2CB80;
extern struct Screen08B2B740 *D_08B2D264;
extern void func_088D3B5C(char *layout, int size);
extern void func_08B2B0E4(struct Screen08B2B740 *self);
extern void func_08B2B22C(struct Screen08B2B740 *self);

struct Screen08B2B740 *func_08B2B740(struct Screen08B2B740 *self)
{
    int i;
    self->vtable = &D_08B2CB80;
    D_08B2D264 = self;
    self->field_A18 = self;
    ((struct Base08B2B740 *)self)->field_000 = 0;
    self->field_A18->field_004 = 0;
    for (i = 0; i < 0x51; i++) {
        self->field_A18->field_49C[i] = 0;
        self->field_A18->field_5E0[i] = 0xFFFF;
    }
    func_088D3B5C(self->field_A18->field_01C, 0x120);
    func_08B2B0E4(self);
    func_08B2B22C(self);
    return self;
}
