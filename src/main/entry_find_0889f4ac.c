int func_0889F5E8(void *table, int key);
void *func_0889F434(void *table, int index);

void *func_0889F4AC(void *table, int key) {
    int index = func_0889F5E8(table, key);
    if (index >= 0) {
        return func_0889F434(table, index);
    } else {
        return 0;
    }
}
