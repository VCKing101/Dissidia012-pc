/* Releases the 12 resource handles of a holder and clears them. */
extern "C" {

typedef struct Holder0892D838 {
    char pad_00[0x44];
    int items[12];         /* 0x44 */
} Holder0892D838;

extern void func_0887BA5C(int item);

void func_0892D838(Holder0892D838 *h)
{
    for (int i = 0; i < 12; i++) {
        if (h->items[i] != 0) {
            func_0887BA5C(h->items[i]);
            h->items[i] = 0;
        }
    }
}

}
