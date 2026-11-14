/* Adds the entry when func_088F6554 accepts it. */
extern "C" {
extern int func_088F6554(void *list, void *key, void *value);
extern void func_088F6698(void *list, void *extra, void *value, void *key);

bool func_088F64DC(void *list, void *key, void *extra, void *value)
{
    if (func_088F6554(list, key, value) == 1) {
        func_088F6698(list, extra, value, key);
        return true;
    }
    return false;
}
}
