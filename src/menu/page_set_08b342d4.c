/* Sets the page (-1 flips between 0 and 1); entering page 0 rebuilds it. */
struct Pages08B342D4 {
    char pad_00[0xB4];
    int field_B4;
};

extern void func_08B338FC(struct Pages08B342D4 *self);
extern void func_08B3361C(struct Pages08B342D4 *self);

void func_08B342D4(struct Pages08B342D4 *self, int page)
{
    int current;
    if (self->field_B4 == page) {
        return;
    }
    if (page == -1) {
        self->field_B4 = self->field_B4 + 1;
        current = self->field_B4 % 2;
        self->field_B4 = current;
    } else {
        self->field_B4 = page;
        current = page;
    }
    switch (current) {
    case 0:
        func_08B338FC(self);
        func_08B3361C(self);
        break;
    case 1:
        break;
    }
}
