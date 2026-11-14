/* Runs check 0x40 on the unit's record from func_088C50D4. */
extern void *func_088C50D4(void *unit);
extern int func_088F4900(int check, int arg, void *record, unsigned char flag);

int func_088F4518(void *unit, unsigned char flag)
{
    return func_088F4900(0x40, 0, func_088C50D4(unit), flag) != 0;
}
