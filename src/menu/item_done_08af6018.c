/* Updates the item and reports whether its icon has finished (no pending frame). */
typedef struct DoneIcons08AF6018 {
    char pad_00[0x5C];
    int field_5C;
} DoneIcons08AF6018;

typedef struct DoneItem08AF6018 {
    char pad_00[0x10];
    DoneIcons08AF6018 *field_10;
} DoneItem08AF6018;

extern void func_08AF6060(void *self, DoneItem08AF6018 *item);

int func_08AF6018(void *self, DoneItem08AF6018 *item)
{
    int done = 0;
    func_08AF6060(self, item);
    if (item->field_10->field_5C == 0) {
        done = 1;
    }
    return done;
}
