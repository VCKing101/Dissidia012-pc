/* Whether the six-entry id stack is full. */
typedef struct IdStack {
    int count;                  /* 0x00 */
} IdStack;

int func_08920244(IdStack *s)
{
    int max = 5;
    if (max < s->count) {
        return 1;
    } else {
        return 0;
    }
}
