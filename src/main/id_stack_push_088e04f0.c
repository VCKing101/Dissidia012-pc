/* Appends an id to the short list at 0x16C0 (count at 0x1700). */
typedef struct IdStack {
    char pad_0000[0x16C0];
    short ids[0x20];
    int count;
    int read;
} IdStack;

void func_088E04F0(IdStack *stack, int id)
{
    stack->ids[stack->count] = id;
    stack->count++;
}
