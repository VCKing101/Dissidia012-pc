/* Free slots: 200 minus the used, counted and extra entries. */
typedef struct Store088C83FC {
    char pad_00[0xBDA0];
    int used;
    int count;
    int extra;
} Store088C83FC;

int func_088C83FC(Store088C83FC *s)
{
    return -s->count - s->used - s->extra + 200;
}
