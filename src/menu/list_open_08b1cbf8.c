/* Shows the list icon (2, or 4 in mode 0), fills the list and starts its
   cursor over the current page's rows. */
struct Cursor08B1CBF8 {
    unsigned int flags;
};

struct State08B1CBF8 {
    char pad_0000[0x488];
    void *field_488;
    char pad_048C[0x1BF8 - 0x48C];
    int field_1BF8[(0x80F8 - 0x1BF8) / 4];
    int field_80F8;
    char pad_80FC[0x8120 - 0x80FC];
    struct Cursor08B1CBF8 field_8120;
};

struct Owner08B1CBF8 {
    char pad_000[0x138];
    struct State08B1CBF8 *field_138;
};

extern void func_088D9F5C(void *part, int id, int show);
extern void func_08B1CCDC(struct Owner08B1CBF8 *self);
extern void func_088D46E4(struct Cursor08B1CBF8 *cursor, int a, int b, int last, int c, int rows);
extern void func_08B1EEC0(struct Owner08B1CBF8 *self);

void func_08B1CBF8(struct Owner08B1CBF8 *self)
{
    struct Cursor08B1CBF8 *cursor;
    int icon = (self->field_138->field_80F8 == 0) ? 4 : 2;
    func_088D9F5C(self->field_138->field_488, icon, 1);
    func_08B1CCDC(self);
    func_088D46E4(&self->field_138->field_8120, 0, 0,
                  self->field_138->field_1BF8[self->field_138->field_80F8] - 1, 0, 9);
    cursor = &self->field_138->field_8120;
    cursor->flags |= 4;
    func_08B1EEC0(self);
}
