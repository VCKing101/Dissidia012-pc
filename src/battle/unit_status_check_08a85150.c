extern void *func_08835798(void *unit);
extern int func_088BF4CC(void *status, int id);

/* True when the unit has status 0x11B. */
int func_08A85150(void *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (func_088BF4CC(func_08835798(unit), 0x11B) == 0) {
        return 0;
    }
    return 1;
}
