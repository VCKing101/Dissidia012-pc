typedef struct QueryOwner {
    char pad_00[0x30];
    char list_30[1];
} QueryOwner;

extern int func_08AE0C30(QueryOwner *self, int mask, int id);
extern int func_08AE9BC8(void *list, int *out, int id);

int func_08AE37B0(QueryOwner *self, int id)
{
    int found;
    if (func_08AE0C30(self, 0x1000, id) == 3) {
        return 0;
    }
    if (func_08AE9BC8(self->list_30, &found, id) == 0) {
        return 0;
    }
    return 1;
}
