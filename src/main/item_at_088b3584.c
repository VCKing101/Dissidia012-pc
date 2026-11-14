/* Registered item i; 0 when out of range or nothing is registered. */
typedef struct ItemInfo088B3584 {
    char pad_00[8];
    int count;
} ItemInfo088B3584;

extern ItemInfo088B3584 *D_089D72E0;
extern int *D_089D72E4;

int func_088B3584(int i)
{
    if (D_089D72E0 == 0) {
        return 0;
    } else if (i < 0 || D_089D72E0->count <= i) {
        return 0;
    } else {
        return D_089D72E4[i];
    }
}
