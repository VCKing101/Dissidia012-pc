/* Finds the effect playing the given resource, optionally with a matching key. */
struct FxNode088AA350;

struct FxList088AA350 {
    char pad_00[0x1C];
    FxNode088AA350 *head;
};

struct FxRes088AA350 {
    char pad_00[0x24];
    int id;
};

struct FxNode088AA350 {
    char pad_00[0xC0];
    FxRes088AA350 *res;
    char pad_C4[0xD0 - 0xC4];
    int key;
    char pad_D4[0x27C - 0xD4];
    FxNode088AA350 *next;

    static FxNode088AA350 *after(FxList088AA350 *list, FxNode088AA350 *node)
    {
        return node == 0 ? list->head : node->next;
    }
};

extern "C" {

FxNode088AA350 *func_088AA350(FxList088AA350 *list, int id, int key)
{
    FxNode088AA350 *node = 0;
    while ((node = FxNode088AA350::after(list, node)) != 0) {
        if (node->res->id == id && (key == 0 || key == node->key)) {
            return node;
        }
    }
    return 0;
}

}
