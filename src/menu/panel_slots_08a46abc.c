/* Panel: clears the four slot marks, then marks the first four of the
 * eight cells whose value is a multiple of 3. */
typedef struct Work46ABC {
    char pad_000[0x3F8];
    int first;                /* +0x3F8 */
    int slots[4];             /* +0x3FC: -1: empty */
    void *field_40C;
    void *field_410;          /* +0x410 */
} Work46ABC;

typedef struct Screen46ABC {
    char pad_000[0x418];
    Work46ABC *work;          /* +0x418 */
} Screen46ABC;

void func_08A46330(Screen46ABC *self, int on, int slot, int value);
int func_08A48450(void *table, int index, int kind);

void func_08A46ABC(Screen46ABC *self)
{
    int i;
    int n;
    int value;

    for (i = 0; i < 4; i++) {
        self->work->slots[i] = -1;
        func_08A46330(self, 0, i, 0);
    }
    n = 0;
    for (i = 0; i < 8; i++) {
        value = func_08A48450(self->work->field_410, self->work->first + i, 3);
        if (value % 3 == 0) {
            self->work->slots[n] = i;
            func_08A46330(self, 1, n, value / 3);
            n++;
            if (n >= 4) {
                break;
            }
        }
    }
}
