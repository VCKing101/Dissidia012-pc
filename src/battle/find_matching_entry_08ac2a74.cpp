/* Walks the list until an entry accepts the owner; returns 1 if one did, else 0. */
extern "C" {
int func_08841FA4(int cur);
int func_08AC2AE8(int owner, int entry);
bool func_08AC2A74(int owner)
{
    int e = 0;
    int one = 1;
    while (1) {
        e = func_08841FA4(e);
        if (e == 0) {
            return false;
        }
        if (func_08AC2AE8(owner, e) == one) {
            return true;
        }
    }
}
}
