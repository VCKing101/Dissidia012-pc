/* Classifies a unit: false for the first three predicate states, true for the next two or special ids 100..103. */
extern "C" {
int func_08A83C40(void *u);
int func_08A83C78(void *u);
int func_08A83CBC(void *u);
int func_088374B0(void *u);
int func_08A83FAC(void *u);
struct U38 { char pad[0x180]; short id; };
bool func_08A83CF4(U38 *u)
{
    if (u == 0) {
        return false;
    }
    if (func_08A83C40(u) == 1) {
        return false;
    }
    if (func_08A83C78(u) == 1) {
        return false;
    }
    if (func_08A83CBC(u) == 1) {
        return false;
    }
    if (func_088374B0(u) == 1) {
        return true;
    }
    if (func_08A83FAC(u) == 1) {
        return true;
    }
    short id = u->id;
    if (id == 100 || id == 0x65 || id == 0x66 || id == 0x67) {
        return true;
    } else {
        return false;
    }
}
}
