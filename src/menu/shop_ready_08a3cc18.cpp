/* True when all eight shop resources have finished loading. */
extern "C" {
int func_08A3C548(void);
int func_08A3C5E8(void);
int func_08A3C720(void);
int func_08A3C7C4(void);
int func_08A3C868(void);
int func_08A3C90C(void);
int func_08A3C9C8(void);
int func_08A3CB5C(void);

bool func_08A3CC18(void)
{
    if (func_08A3C548() == 0) {
        return false;
    }
    if (func_08A3C5E8() == 0) {
        return false;
    }
    if (func_08A3C720() == 0) {
        return false;
    }
    if (func_08A3C7C4() == 0) {
        return false;
    }
    if (func_08A3C868() == 0) {
        return false;
    }
    if (func_08A3C90C() == 0) {
        return false;
    }
    if (func_08A3C9C8() == 0) {
        return false;
    }
    if (func_08A3CB5C() == 0) {
        return false;
    }
    return true;
}
}
