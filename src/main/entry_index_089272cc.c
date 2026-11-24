/* Returns the position, among entries of the given kind that are in use, of the entry whose id matches; 0 if none. */
typedef struct Ent { char pad[0x16]; char kind; char pad2[3]; short id; short used; char pad3[0x30]; } Ent;
typedef struct Tbl { char pad[0x10]; short count; char pad2[6]; Ent ents[1]; } Tbl;
unsigned short func_089272CC(int self, int kind, short id)
{
    unsigned short result = 0;
    int pos = 0;
    int i = 0;
    Tbl *t = *(Tbl **)(self + 0xC);
    if (i < t->count) {
        Ent *e = (Ent *)t;
        do {
            if (e->kind == kind && e->used != -1) {
                if (e->id == id) {
                    result = pos;
                    break;
                }
                pos++;
            }
            i++;
            e = (Ent *)((char *)e + 0x4E);
        } while (i < t->count);
    }
    return result;
}
