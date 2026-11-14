/* Shows (`on` 1) or hides the list at 0xE0C. */
struct State08B3263C {
    char pad_000[0xE0C];
    char field_E0C[4];
};

struct Owner08B3263C {
    char pad_000[0x138];
    struct State08B3263C *field_138;
};

extern void func_088D4FE4(char *list);
extern void func_088D50DC(char *list);

void func_08B3263C(struct Owner08B3263C *self, unsigned char on)
{
    if (on == 1) {
        func_088D4FE4(self->field_138->field_E0C);
    } else {
        func_088D50DC(self->field_138->field_E0C);
    }
}
