/* The value while the state is 1, otherwise 0. */
typedef struct Obj088A1C38 {
    float value;
    char pad_04[8];
    unsigned char state;
} Obj088A1C38;

float func_088A1C38(Obj088A1C38 *o)
{
    if (o->state == 1) {
        return o->value;
    }
    return 0.0f;
}
