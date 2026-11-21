/* True when the unit lacks flags 0x38 and 0x3A and has 0x39 or 0x11C. */
extern "C" {
int func_08B2FBCC(void *u, int a);
int func_08835778(void *u);
int func_088BF4CC(int v, int flag);
bool func_08A853CC(void *u)
{
    int v;
    if (u == 0) {
        return false;
    } else {
        if (func_08B2FBCC(u, 0) == 0) {
            return false;
        } else {
            v = func_08835778(u);
            if (func_088BF4CC(v, 0x38) == 0) {
                if (func_088BF4CC(v, 0x3A) == 0) {
                    if (func_088BF4CC(v, 0x39) == 0) {
                        if (func_088BF4CC(v, 0x11C) != 0) {
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return true;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
}
}
