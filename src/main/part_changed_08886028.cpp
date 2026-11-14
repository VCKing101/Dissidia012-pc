struct Row { unsigned short field_00; unsigned short field_02; unsigned short id; };
struct Part { char data_00[0xC]; short id; char data_0E[0x20 - 0xE]; unsigned short flags;
    static bool ready(Part *p) { return (p->flags & 0x20) != 0; } };
struct Owner { int field_00; Part *part; int kind; };
extern "C" {
extern Row D_089C9FD8[];
int func_08886028(Owner *o)
{
    Part *p = o->part;
    if (p != 0) {
        if (p->id == D_089C9FD8[o->kind].id && Part::ready(p)) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
}
