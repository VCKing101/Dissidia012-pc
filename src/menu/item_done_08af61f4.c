/* Updates the item and reports whether its icon has finished (no pending frame). */
typedef struct DoneIcons08AF61F4 {
    char pad_00[0x5C];
    int field_5C;
} DoneIcons08AF61F4;

typedef struct DoneItem08AF61F4 {
    char pad_00[0x10];
    DoneIcons08AF61F4 *field_10;
} DoneItem08AF61F4;

extern void func_08AF623C(void *self, DoneItem08AF61F4 *item);

int func_08AF61F4(void *self, DoneItem08AF61F4 *item)
{
    int done = 0;
    func_08AF623C(self, item);
    if (item->field_10->field_5C == 0) {
        done = 1;
    }
    return done;
}
