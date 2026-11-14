/* Clears every row's column mark, then marks the rows used by the five
   columns of set `set` (0x7FFF is an empty column). */
struct Row08AEC700 {
    char pad_0[8];
    signed char field_8;
    char pad_9[3];
};

struct State08AEC700 {
    char pad_0000[0x19DF];
    unsigned char field_19DF;
    struct Row08AEC700 field_19E0[(0x1CE0 - 0x19E0) / 12];
    unsigned short field_1CE0[1][5];
};

struct Screen08AEC700 {
    char pad_0000[0x23F0];
    struct State08AEC700 *field_23F0;
};

void func_08AEC700(struct Screen08AEC700 *self, int set)
{
    int i;
    for (i = 0; i < self->field_23F0->field_19DF; i++) {
        self->field_23F0->field_19E0[i].field_8 = -1;
    }
    for (i = 0; i < 5; i++) {
        unsigned short row = self->field_23F0->field_1CE0[set][i];
        if (row != 0x7FFF) {
            self->field_23F0->field_19E0[row].field_8 = i;
        }
    }
}
