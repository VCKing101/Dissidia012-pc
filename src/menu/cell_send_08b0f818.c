/* Forwards to func_08B0F700 with the state's field 0x830. */
struct State08B0F818 {
    char pad_000[0x830];
    int field_830;
};

struct Owner08B0F818 {
    char pad_000[0x138];
    struct State08B0F818 *field_138;
};

extern void func_08B0F700(struct Owner08B0F818 *self, int a, int b, int c);

void func_08B0F818(struct Owner08B0F818 *self, int a, int b)
{
    func_08B0F700(self, a, b, self->field_138->field_830);
}
