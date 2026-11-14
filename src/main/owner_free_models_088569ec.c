typedef struct Rec088569EC { int model; char pad_04[0x1C]; } Rec088569EC;
typedef struct Owner088569EC { char pad_000[0x78]; Rec088569EC recs[6]; void *item; } Owner088569EC;
extern void func_088DAB44(void *item);
extern void func_0887BA5C(int model);
void func_088569EC(Owner088569EC *o)
{
    int i;
    if (o->item != 0) {
        func_088DAB44(o->item);
        o->item = 0;
    }
    for (i = 0; i < 6; i++) {
        if (o->recs[i].model != 0) {
            func_0887BA5C(o->recs[i].model);
            o->recs[i].model = 0;
        }
    }
}
