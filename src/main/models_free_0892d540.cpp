/* Releases the 5 model handles of a holder and clears them. */
extern "C" {

typedef struct Holder0892D540 {
    char pad_00[0x30];
    void *items[5];        /* 0x30 */
} Holder0892D540;

extern void func_088DA060(void *item);

void func_0892D540(Holder0892D540 *h)
{
    for (int i = 0; i < 5; i++) {
        if (h->items[i] != 0) {
            func_088DA060(h->items[i]);
            h->items[i] = 0;
        }
    }
}

}
