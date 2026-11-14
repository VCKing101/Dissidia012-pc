extern int func_08857C6C(void *dst, int kind);
int func_0885A574(char *self, int kind)
{
    int result = 0;
    if (kind == 0 || kind == 1) {
        result = func_08857C6C(self + 0x52D0, kind);
    }
    return result;
}
