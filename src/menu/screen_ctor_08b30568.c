/* Constructor: vtable, self link, cleared preview table; then loads the
   layout. */
/* The object's first word, as the base part sees it. */
struct Base08B30568 {
    int field_000;
};

struct Screen08B30568 {
    int field_000;
    int field_004;
    char pad_008[0x10 - 8];
    char field_010[0x490 - 0x10];
    void *field_490[0x6D];
    unsigned short field_644[0x6D];
    char pad_71E[0x9D8 - 0x71E];
    struct Screen08B30568 *field_9D8;
    void *vtable;
};

extern int D_08B310E8;
extern void func_088D3B5C(char *layout, int size);

struct Screen08B30568 *func_08B30568(struct Screen08B30568 *self)
{
    int i;
    self->vtable = &D_08B310E8;
    self->field_9D8 = self;
    ((struct Base08B30568 *)self)->field_000 = 0;
    self->field_9D8->field_004 = 0;
    for (i = 0; i < 0x6D; i++) {
        self->field_9D8->field_490[i] = 0;
        self->field_9D8->field_644[i] = 0xFFFF;
    }
    func_088D3B5C(self->field_9D8->field_010, 0x120);
    return self;
}
