/* Resolves an item id: -1 takes the next id from the stack, -2 means none. */
typedef struct IdItem {
    char pad_00[8];
    int id;
} IdItem;

typedef struct IdResolver {
    void *stack;
} IdResolver;

extern int func_088E050C(void *stack);

int func_088E38D8(IdResolver *resolver, IdItem *item)
{
    int id = item->id;

    if (id == -1) {
        return func_088E050C(resolver->stack);
    }
    if (id == -2) {
        return -1;
    }
    return id;
}
