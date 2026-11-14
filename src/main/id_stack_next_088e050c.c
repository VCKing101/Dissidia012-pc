/* Returns the next id from the list at 0x16C0 (read position at 0x1704). */
typedef struct IdStack {
    char pad_0000[0x16C0];
    short ids[0x20];
    int count;
    int read;
} IdStack;

short func_088E050C(IdStack *stack)
{
    return stack->ids[stack->read++];
}
