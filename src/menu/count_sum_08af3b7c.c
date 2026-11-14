/* Sums the list counts for entries 1 .. count - 1. */
extern int func_088C08BC(void *list, int index);

int func_08AF3B7C(void *self, void *list, int count)
{
    int total = 0;
    int i = 1;
    for (; i < count; i++) {
        total += func_088C08BC(list, i);
    }
    return total;
}
