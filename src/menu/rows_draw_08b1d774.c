/* Draws the window frame and title, every row's label and value, then the
   tint. */
struct Label08B1D774 {
    char data[0x50];
};

struct Value08B1D774 {
    char data[0x38];
};

struct State08B1D774 {
    char pad_0000[4];
    char field_004[0x400];
    char field_404[0x884 - 0x404];
    char field_884[0x8198 - 0x884];
    char field_8198[0x81F0 - 0x8198];
    struct Label08B1D774 field_81F0[10];
    struct Value08B1D774 field_8510[10];
};

struct Owner08B1D774 {
    char pad_000[0x138];
    struct State08B1D774 *field_138;
};

extern unsigned short D_08B224F0[];
extern void func_088D3F00(char *owner, char *layout, void *text);
extern void func_088D44EC(char *layout, char *cursor, void *value);
extern void func_08B1DA58(struct Owner08B1D774 *self);

void func_08B1D774(struct Owner08B1D774 *self)
{
    int i;
    func_088D3F00(self->field_138->field_004, self->field_138->field_404, D_08B224F0);
    func_088D3F00(self->field_138->field_004, self->field_138->field_404, self->field_138->field_8198);
    for (i = 0; i < 10; i++) {
        func_088D3F00(self->field_138->field_004, self->field_138->field_404, &self->field_138->field_81F0[i]);
        func_088D44EC(self->field_138->field_404, self->field_138->field_884, &self->field_138->field_8510[i]);
    }
    func_08B1DA58(self);
}
