/* Pushes an id onto a six-entry stack when the count is in range.
   Retail compares against the limit held in a register (`ori 5; slt`). */
typedef struct IdStack {
    int count;                  /* 0x00 */
    int ids[6];                 /* 0x04 */
} IdStack;

void func_089200F0(IdStack *s, int id)
{
    int n = s->count;
    int max = 5;
    if (n < 0 || max < n) {
        return;
    }
    {
        int *slot = &s->ids[n];
        *slot = id;
        s->count++;
    }
}
