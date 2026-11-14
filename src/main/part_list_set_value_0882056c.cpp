/* Applies a value to every part of a loaded part list; -1 when the list is
   not loaded. */
extern "C" {

typedef struct Part {
    char data[0x3C];
} Part;

typedef struct PartList {
    short id;                   /* 0x0 */
    short count;                /* 0x2 */
    Part *parts;                /* 0x4 */
    static bool loaded(PartList *l) { return l->id != 0; }
} PartList;

extern void func_08820A14(Part *part, int value);

int func_0882056C(PartList *l, int value)
{
    if (!PartList::loaded(l)) {
        return -1;
    }
    for (int i = 0; i < l->count; i++) {
        func_08820A14(&l->parts[i], value);
    }
    return 0;
}

}
