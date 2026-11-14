/* Stops every effect in the owner's list whose owner key matches. */
struct FxNode088A9D7C;

struct FxList088A9D7C {
    char pad_00[0x1C];
    FxNode088A9D7C *head;
};

struct FxNode088A9D7C {
    char pad_00[0xD0];
    int key;
    char pad_D4[0x27C - 0xD4];
    FxNode088A9D7C *next;

    static FxNode088A9D7C *after(FxList088A9D7C *list, FxNode088A9D7C *node)
    {
        return node == 0 ? list->head : node->next;
    }
};

extern "C" {

void func_088A6C70(FxNode088A9D7C *node);

void func_088A9D7C(FxList088A9D7C *list, int key)
{
    FxNode088A9D7C *node = 0;
    while ((node = FxNode088A9D7C::after(list, node)) != 0) {
        if (node->key == key) {
            func_088A6C70(node);
        }
    }
}

}
