/* Frees every effect in the list (list and cursor kept in the frame across the call). */
struct FxNode088AA154;

struct FxList088AA154 {
    char pad_00[0x1C];
    FxNode088AA154 *head;
};

struct FxNode088AA154 {
    char pad_00[0x27C];
    FxNode088AA154 *next;

    static FxNode088AA154 *after(FxList088AA154 *list, FxNode088AA154 *node)
    {
        return node == 0 ? list->head : node->next;
    }
};

extern "C" {

void func_088A6A78(FxNode088AA154 *node);

void func_088AA154(FxList088AA154 *list)
{
    FxNode088AA154 *node = 0;
    while ((node = FxNode088AA154::after(list, node)) != 0) {
        FxList088AA154 *saved_list[1];
        FxNode088AA154 *saved_node[1];
        saved_node[0] = node;
        saved_list[0] = list;
        func_088A6A78(node);
        list = saved_list[0];
        node = saved_node[0];
    }
}

}
