/* 1 when the value is negative and the unit is not in states 4, 5, 7 or 8. */
typedef struct Unit088ACD8C {
    char pad_00[0x210];
    int state;
} Unit088ACD8C;

int func_088ACD8C(Unit088ACD8C *unit, int value)
{
    if (value < 0) {
        int state = unit->state;
        if (state != 5 && state != 4 && state != 7 && state != 8) {
            return 1;
        }
    }
    return 0;
}
