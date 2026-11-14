/* Shows the two mode icons (4/0 or 3/1) and retints the model. */
struct State08B091C4 {
    char pad_00000[0x6EC];
    void *field_6EC;
    void *field_6F0;
    char pad_006F4[0x1F960 - 0x6F4];
    int field_1F960;
};

struct Owner08B091C4 {
    char pad_000[0x138];
    struct State08B091C4 *field_138;
};

extern void func_088D9F5C(void *part, int id, int show);
extern void func_08B09238(struct Owner08B091C4 *self);

void func_08B091C4(struct Owner08B091C4 *self)
{
    int first = 4;
    int second = 0;
    if (self->field_138->field_1F960 != 0) {
        first = 3;
        second = 1;
    }
    func_088D9F5C(self->field_138->field_6EC, first, 1);
    func_088D9F5C(self->field_138->field_6F0, second, 1);
    func_08B09238(self);
}
