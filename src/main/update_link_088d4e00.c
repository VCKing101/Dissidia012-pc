/* Set the pending target; the first change also marks it dirty and releases the old target's slot. */
typedef struct Rec { char pad[4]; unsigned short v; char pad2[0x1E - 6]; } Rec;
extern void func_088D9F5C(unsigned a, int b, int c);
void func_088D4E00(unsigned target, unsigned *o)
{
    unsigned cur;
    if ((o[0] & 1) != 0) {
        if (o[2] != target) {
            o[2] = target;
            return;
        }
        return;
    }
    cur = o[1];
    if (cur == target) {
        return;
    }
    if (cur != 0xFFFFFFFF && target == 0xFFFFFFFF && o[4] != 0) {
        func_088D9F5C(o[4], ((Rec *)o[9])[cur].v, 1);
    }
    o[2] = target;
    o[0] = o[0] | 1;
}
