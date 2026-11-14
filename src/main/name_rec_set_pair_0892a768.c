typedef struct NamePair { int a; int b; } NamePair;
typedef struct NameRec { short count; short used; char name[0x1A]; char pad_1E[2]; NamePair pairs[5]; } NameRec;
extern void func_0892A654(NameRec *rec);
void func_0892A768(NameRec *rec, int index, int a, int b)
{
    rec->pairs[index].a = a;
    rec->pairs[index].b = b;
    func_0892A654(rec);
}
