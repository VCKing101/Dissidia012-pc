typedef struct NameRec { char data[0x48]; } NameRec;
typedef struct NameTable { int count; NameRec recs[5]; } NameTable;
extern void func_08935828(void *dst, int value, int size);
extern void func_0892A5EC(NameRec *rec);
void func_0892A8B8(NameTable *t)
{
    int i;
    t->count = 0;
    func_08935828(t->recs, 0, 0x168);
    for (i = 0; i < 5; i++) {
        func_0892A5EC(&t->recs[i]);
    }
}
