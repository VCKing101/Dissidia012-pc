typedef struct FloatHolder2C {
    char pad_00[0x2C];
    float value;
} FloatHolder2C;

/* The holder's value at 0x2C, 0 without a holder. */
float func_08AB3414(FloatHolder2C *holder)
{
    if (holder == 0) {
        return 0.0f;
    }
    return holder->value;
}
