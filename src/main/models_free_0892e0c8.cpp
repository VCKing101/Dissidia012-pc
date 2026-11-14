/* Releases the 2 model handles of a holder and clears them. */
extern "C" {

typedef struct Holder0892E0C8 {
    char pad_00[0x30];
    void *items[2];        /* 0x30 */
} Holder0892E0C8;

extern void func_088DA060(void *item);

void func_0892E0C8(Holder0892E0C8 *h)
{
    for (int i = 0; i < 2; i++) {
        if (h->items[i] != 0) {
            func_088DA060(h->items[i]);
            h->items[i] = 0;
        }
    }
}

}
