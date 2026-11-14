/* Shows (`on` 1) or hides the list at 0xE18. */
struct State08B29558 {
    char pad_000[0xE18];
    char field_E18[4];
};

struct Owner08B29558 {
    char pad_000[0x14C];
    struct State08B29558 *field_14C;
};

extern void func_088D4FE4(char *list);
extern void func_088D50DC(char *list);

void func_08B29558(struct Owner08B29558 *self, unsigned char on)
{
    if (on == 1) {
        func_088D4FE4(self->field_14C->field_E18);
    } else {
        func_088D50DC(self->field_14C->field_E18);
    }
}
