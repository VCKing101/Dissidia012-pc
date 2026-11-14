/* Runs check 0x40 on the unit's record from func_088C5140. */
extern void *func_088C5140(void *unit);
extern int func_088F4900(int check, int arg, void *record, unsigned char flag);

int func_088F44DC(void *unit, unsigned char flag)
{
    return func_088F4900(0x40, 0, func_088C5140(unit), flag) != 0;
}
