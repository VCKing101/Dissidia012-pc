typedef struct FloatHolder24 {
    char pad_00[0x24];
    float value;
} FloatHolder24;

/* The holder's value at 0x24, 0 without a holder. */
float func_08AB33DC(FloatHolder24 *holder)
{
    if (holder == 0) {
        return 0.0f;
    }
    return holder->value;
}
