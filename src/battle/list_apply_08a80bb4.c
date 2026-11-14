typedef struct ListOwner {
    unsigned char active;
} ListOwner;

extern void *func_08899300(void);
extern unsigned int func_08898194(void *list);
extern void *func_088980E0(void *list, unsigned int index);
extern int func_08A80C60(ListOwner *self, void *item);

/* Applies list item index; -1 inactive, -2 out of range, -3 no item, -4 apply failed. */
int func_08A80BB4(ListOwner *self, unsigned int index)
{
    void *list;
    void *item;
    if (self->active == 0) {
        return -1;
    }
    list = func_08899300();
    if (index >= func_08898194(list)) {
        return -2;
    }
    item = func_088980E0(list, index);
    if (item == 0) {
        return -3;
    }
    if (func_08A80C60(self, item) != 0) {
        return -4;
    }
    return 0;
}
