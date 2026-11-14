/* Advances the result screen once the input part is done (flag 0x10), then refreshes and sets state 15. */
struct ResultState {
    char pad_000[0x910];
    int field_910;
    int field_914;
    char pad_918[0x7740];
    unsigned char field_8058;
    char pad_8059[0x15B];
    unsigned int field_81B4;
};

struct ResultOwner {
    char pad_000[0x154];
    ResultState *field_154;
    char pad_158[0x894 - 0x158];
    char field_894[0x4];
};

extern "C" {
int func_08AB6E14(char *input);
int func_08AB6E38(char *input);
int func_08AB5E58(char *input);
void func_08AB6E5C(char *input);
void func_08AB6D20(char *input, int mode);
void func_08A9A750(ResultOwner *self);
void func_08A9D080(ResultOwner *self);
void func_08A9A6E0(ResultOwner *self);

void func_08A987BC(ResultOwner *self)
{
    if ((self->field_154->field_81B4 & 0x10) != 0) {
        char *input = self->field_894;
        if (func_08AB6E14(input) == 1) {
            if (func_08AB6E38(input) == 0) {
                return;
            }
            self->field_154->field_8058 = 0;
            self->field_154->field_914 = 0x30;
        } else {
            if (func_08AB5E58(input) == 0) {
                return;
            }
            func_08AB6E5C(input);
            func_08AB6D20(input, 1);
        }
    }
    func_08A9A750(self);
    func_08A9D080(self);
    func_08A9A6E0(self);
    self->field_154->field_910 = 0xF;
}
}
