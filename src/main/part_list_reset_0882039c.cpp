extern "C" {
typedef struct Part { char data[0x3C]; } Part;
typedef struct PartList {
    short id;
    short count;
    Part *parts;
    static bool loaded(PartList *l) { return l->id != 0; }
} PartList;
extern void func_08820750(Part *part);
int func_0882039C(PartList *l)
{
    if (!PartList::loaded(l)) {
        return -1;
    }
    for (int i = 0; i < l->count; i++) {
        func_08820750(&l->parts[i]);
    }
    return 0;
}
}
