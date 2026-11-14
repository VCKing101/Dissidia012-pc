typedef struct FloatHolder30 {
    char pad_00[0x30];
    float value;
} FloatHolder30;

/* The holder's value at 0x30, 0 without a holder. */
float func_08AB3430(FloatHolder30 *holder)
{
    if (holder == 0) {
        return 0.0f;
    }
    return holder->value;
}
