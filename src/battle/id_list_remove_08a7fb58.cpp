typedef struct IdList {
    char pad_00[0x8];
    int ids[3];
} IdList;

extern "C" int func_08A7FB58(void *self, IdList *list, unsigned int id)
{
    int result = -1;
    if (list == 0 || id >= 19) {
        return result;
    }
    for (int i = 0; i < 3;) {
        if (list->ids[i] != id) {
            i++;
        } else {
            for (int j = i; j < 2; j++) {
                list->ids[j] = list->ids[j + 1];
            }
            list->ids[2] = result;
        }
    }
    return 0;
}
