/* Panel: shows the cells of the eight rows from first: the three kinds of
 * each with their flags (2: on, 4: off), then the row total. */
typedef struct Work46538 {
    char pad_000[0x410];
    void *table;              /* +0x410 */
} Work46538;

typedef struct Screen46538 {
    char pad_000[0x418];
    Work46538 *work;          /* +0x418 */
} Screen46538;

int func_08A48450(void *table, int index, int kind);
int func_08A484D8(void *table, int a, int value);
void func_08A46238(Screen46538 *self, int on, int row, int kind, int a, int b);
void func_08A461E8(Screen46538 *self, int row, int count, int rest);

void func_08A46538(Screen46538 *self, unsigned int first)
{
    int row;
    int kind;

    int value;
    int flags;
    int on[2];

    for (row = 0; row < 8; row++) {

        for (kind = 0; kind < 3; kind++) {
            func_08A46238(self, 0, row, kind, 0, 0);
            value = func_08A48450(self->work->table, row + first, kind);
            if (value != -1) {
                flags = func_08A484D8(self->work->table, 0, value);
                on[0] = 0;
                if (flags & 2) {
                    on[0] = 1;
                }
                on[1] = 1;
                if (flags & 4) {
                    on[1] = 0;
                }
                func_08A46238(self, 1, row, kind, on[0], on[1]);
            }
        }
        value = func_08A48450(self->work->table, row + first, 3);
        func_08A461E8(self, row, value / 3, value % 3);
    }
}
