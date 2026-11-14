/* Shows the two mode icons (3/0 or 2/1) and retints the model. */
struct State08B0712C {
    char pad_00000[0x6D8];
    void *field_6D8;
    void *field_6DC;
    char pad_006E0[0x1F98C - 0x6E0];
    int field_1F98C;
};

struct Owner08B0712C {
    char pad_000[0x138];
    struct State08B0712C *field_138;
};

extern void func_088D9F5C(void *part, int id, int show);
extern void func_08B071A0(struct Owner08B0712C *self);

void func_08B0712C(struct Owner08B0712C *self)
{
    int first = 3;
    int second = 0;
    if (self->field_138->field_1F98C != 0) {
        first = 2;
        second = 1;
    }
    func_088D9F5C(self->field_138->field_6D8, first, 1);
    func_088D9F5C(self->field_138->field_6DC, second, 1);
    func_08B071A0(self);
}
