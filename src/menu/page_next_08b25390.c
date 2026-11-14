/* When paging is on, sends the next page number (wrapping to 0). */
struct State08B25390 {
    char pad_000[0x1AC];
    unsigned int field_1AC;
    char pad_1B0[0x1D0 - 0x1B0];
    int field_1D0;
    int field_1D4;
};

struct Owner08B25390 {
    struct State08B25390 *field_0;
};

extern void *D_08B01CF8;
extern void func_08AF44AC(void *queue, int page);

void func_08B25390(struct Owner08B25390 *self)
{
    struct State08B25390 *state = self->field_0;
    if ((state->field_1AC & 1) != 0) {
        int next[2];
        next[0] = state->field_1D0 + 1;
        if (next[0] == state->field_1D4) {
            next[0] = 0;
        }
        func_08AF44AC(D_08B01CF8, next[0]);
    }
}
