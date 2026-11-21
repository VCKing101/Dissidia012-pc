/* Panel: when the cell has no value of kind, takes the first of the two
 * fallback kinds (D_08A481CC by column) that has one. */
typedef struct Work4668C {
    char pad_000[0x3F4];
    int field_3F4;            /* +0x3F4 */
    int first;                /* +0x3F8 */
    char pad_3FC[0x14];
    void *table;              /* +0x410 */
} Work4668C;

typedef struct Screen4668C {
    char pad_000[0x418];
    Work4668C *work;          /* +0x418 */
} Screen4668C;

extern short D_08A481CC[];
int func_08A48450(void *table, int index, int kind);

void func_08A4668C(Screen4668C *self, int cell, int kind, int column)
{
    int result[2];
    int i;
    short *pair;

    result[0] = func_08A48450(self->work->table, self->work->first + cell, kind);
    if (result[0] == -1) {
        i = 0;
        pair = &D_08A481CC[column * 6 + kind * 2];
        for (; i < 2; i++) {
            result[1] = func_08A48450(self->work->table, self->work->first + cell, pair[i]);
            if (result[1] != -1) {
                self->work->field_3F4 = pair[i];
                return;
            }
        }
    }
}
