typedef struct FloatHolder28 {
    char pad_00[0x28];
    float value;
} FloatHolder28;

/* The holder's value at 0x28, 0 without a holder. */
float func_08AB33F8(FloatHolder28 *holder)
{
    if (holder == 0) {
        return 0.0f;
    }
    return holder->value;
}
