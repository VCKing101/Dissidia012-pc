typedef struct Item { int key; int h; int pad[5]; } Item;
typedef struct List { char pad[8]; int count; int pad4; Item items[1]; } List;
typedef struct Own { List *list; } Own;
extern void func_088A27D0(float a, float b, int h);
void func_08896198(float a, float b, Own *self, int key) {
    List *l = self->list;
    if (l != 0) {
        Item *it = l->items;
        int i = 0;
        if (i < l->count) {
            do {
                if (it->key == key) {
                    func_088A27D0(a, b, it->h);
                }
                i++;
                it++;
            } while (i < self->list->count);
        }
    }
}
