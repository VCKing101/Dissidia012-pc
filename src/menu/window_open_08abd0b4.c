/* Opens the window: loads its layout and text, starts the cursor (enabled
   unless the mode is 2), then sets flags and the starting row. */
struct Cursor08ABD0B4 {
    unsigned int flags;
};

struct Window08ABD0B4 {
    char pad_000[0x400];
    char field_400[0x880 - 0x400];
    struct Cursor08ABD0B4 field_880;
    char pad_884[0x8F4 - 0x884];
    char field_8F4[0xA58 - 0x8F4];
    unsigned int field_A58;
    int field_A5C;
    int field_A60;
    char pad_A64[0xB58 - 0xA64];
    unsigned char field_B58;
};

struct Page08ABD0B4 {
    char pad_000[0x14C];
    struct Window08ABD0B4 *field_14C;
};

extern unsigned short D_08AC43CE;
extern unsigned short D_08AC43FE;
extern void func_088D3CF0(struct Window08ABD0B4 *window, char *layout, unsigned short *table);
extern void func_088D428C(char *layout, char *text, unsigned short *table, int show);
extern void func_088D46E4(struct Cursor08ABD0B4 *cursor, int enabled, int a, int b, int c, int d);
extern void func_08ABD17C(struct Page08ABD0B4 *self);
extern int func_08ABC868(struct Page08ABD0B4 *self);

void func_08ABD0B4(struct Page08ABD0B4 *self)
{
    struct Window08ABD0B4 *window;
    int enabled;
    struct Cursor08ABD0B4 *cursor;
    func_088D3CF0(self->field_14C, self->field_14C->field_400, &D_08AC43CE);
    func_088D428C(self->field_14C->field_400, self->field_14C->field_8F4, &D_08AC43FE, 1);
    window = self->field_14C;
    enabled = 1;
    if (window->field_A60 == 2) {
        enabled = 0;
    }
    window->field_B58 = 0;
    func_088D46E4(&self->field_14C->field_880, enabled, 0, 1, 0, -1);
    cursor = &self->field_14C->field_880;
    cursor->flags |= 1;
    func_08ABD17C(self);
    self->field_14C->field_A58 |= 0x20;
    self->field_14C->field_A5C = func_08ABC868(self);
}
