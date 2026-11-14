/* Finds the registered item with the given id byte; 0 when none or nothing is registered. */
struct ItemInfo088B3524 {
    char pad_00[8];
    int count;
};

struct Item088B3524 {
    signed char id;
};

extern "C" {

extern ItemInfo088B3524 *D_089D72E0;
extern Item088B3524 **D_089D72E4;

Item088B3524 *func_088B3524(int id)
{
    Item088B3524 *result = 0;
    if (D_089D72E0 == 0) {
        return 0;
    }
    for (int i = 0; i < D_089D72E0->count; i++) {
        Item088B3524 *item = D_089D72E4[i];
        if (item->id == id) {
            result = item;
            break;
        }
    }
    return result;
}

}
