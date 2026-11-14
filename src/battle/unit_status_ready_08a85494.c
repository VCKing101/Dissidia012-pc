extern int func_08B2FBCC(void *unit, int arg);
extern void *func_08835778(void *unit);
extern int func_088BF4CC(void *status, int id);

/* True when the unit is ready and has status 0x11C. */
int func_08A85494(void *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (func_08B2FBCC(unit, 0) == 0) {
        return 0;
    }
    if (func_088BF4CC(func_08835778(unit), 0x11C) != 0) {
        return 1;
    }
    return 0;
}
