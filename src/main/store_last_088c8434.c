/* Index of the last counted slot; -1 when out of range. */
typedef struct Store088C8434 {
    char pad_00[0xBDA0];
    int used;
    unsigned int count;
    int extra;
} Store088C8434;

unsigned int func_088C8434(Store088C8434 *s)
{
    unsigned int i = 199 - s->count;
    if (i >= 200) {
        return -1;
    }
    return i;
}
