typedef struct LoadPart LoadPart;

typedef struct LoadOwner60 {
    char pad_00[0x60];
    LoadPart *part_60[1];
} LoadOwner60;

extern void func_08ABAE5C(LoadOwner60 *self);
extern int func_08ABC118(void *part, void *data, int size);

/* Reset, then load size bytes of data into the part at 0x60; -1 for no data, -2 when loading fails. */
int func_08ABB00C(LoadOwner60 *self, void *data, int size)
{
    func_08ABAE5C(self);
    if (data == 0 || size <= 0) {
        return -1;
    }
    if (func_08ABC118(self->part_60, data, size) != 0) {
        return -2;
    }
    return 0;
}
