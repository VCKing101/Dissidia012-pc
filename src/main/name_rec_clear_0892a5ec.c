typedef struct NamePair { int a; int b; } NamePair;
typedef struct NameRec { short count; short used; char name[0x1A]; char pad_1E[2]; NamePair pairs[5]; } NameRec;
extern void func_08935828(void *dst, int value, int size);
void func_0892A5EC(NameRec *rec)
{
    int i;
    rec->count = 0;
    rec->used = 0;
    func_08935828(rec->name, 0, 0x1A);
    func_08935828(rec->pairs, 0, 0x28);
    for (i = 0; i < 5; i++) {
        rec->pairs[i].a = 0;
        rec->pairs[i].b = 0;
    }
}
