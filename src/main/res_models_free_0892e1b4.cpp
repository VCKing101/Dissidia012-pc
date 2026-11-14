/* Releases the 1 resource handles of a holder and clears them. */
extern "C" {

typedef struct Holder0892E1B4 {
    char pad_00[0x38];
    int items[1];         /* 0x38 */
} Holder0892E1B4;

extern void func_0887BA5C(int item);

void func_0892E1B4(Holder0892E1B4 *h)
{
    for (int i = 0; i < 1; i++) {
        if (h->items[i] != 0) {
            func_0887BA5C(h->items[i]);
            h->items[i] = 0;
        }
    }
}

}
