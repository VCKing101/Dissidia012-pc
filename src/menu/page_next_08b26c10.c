/* Sends the next page number (wrapping to 0) and the page data, then marks
   the page sent. */
struct State08B26C10 {
    char pad_000[0x1AC];
    unsigned int field_1AC;
    char pad_1B0[0x1D0 - 0x1B0];
    int field_1D0;
    int field_1D4;
    char pad_1D8[0x29C - 0x1D8];
    char field_29C[4];
};

struct Owner08B26C10 {
    struct State08B26C10 *field_0;
};

extern void *D_08B01CF8;
extern void func_08AF44AC(void *queue, int page);
extern void func_08AFF57C(void *queue, char *data);

void func_08B26C10(struct Owner08B26C10 *self)
{
    int next[2];
    next[0] = self->field_0->field_1D0 + 1;
    if (next[0] == self->field_0->field_1D4) {
        next[0] = 0;
    }
    func_08AF44AC(D_08B01CF8, next[0]);
    func_08AFF57C(D_08B01CF8, self->field_0->field_29C);
    self->field_0->field_1AC |= 2;
}
