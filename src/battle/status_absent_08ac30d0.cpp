extern "C" {
extern void *func_08835778(void *unit);
extern int func_088BF4CC(void *status, int id);
}

struct StatusCheck {
    static bool has(void *unit) { return func_088BF4CC(func_08835778(unit), 0xED) != 0; }
};

extern "C" int func_08AC30D0(void *self, void *unit)
{
    if (unit == 0) {
        return 0;
    }
    return !StatusCheck::has(unit);
}
