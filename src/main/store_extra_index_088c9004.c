/* Slot index of extra entry i counted from the top; 0 when out of range. */
typedef struct Store088C9004 {
    char pad_00[0xBDA0];
    int used;
    int count;
    unsigned int extra;
} Store088C9004;

int func_088C9004(Store088C9004 *s, unsigned int i)
{
    if (i >= s->extra) {
        return 0;
    }
    return -i - s->count + 199;
}
