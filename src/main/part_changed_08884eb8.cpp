struct Row { unsigned short field_00; unsigned short field_02; unsigned short id; unsigned short field_06; unsigned short field_08; };
struct Part { char data_00[0xC]; short id; char data_0E[0x20 - 0xE]; unsigned short flags;
    static bool ready(Part *p) { return (p->flags & 0x20) != 0; } };
struct Owner { int field_00; Part *part; char data_08[0x14 - 0x8]; int kind; };
extern "C" {
extern Row D_089C9F88[];
int func_08884EB8(Owner *o)
{
    Part *p = o->part;
    if (p != 0) {
        if (p->id == D_089C9F88[o->kind].id && Part::ready(p)) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
}
