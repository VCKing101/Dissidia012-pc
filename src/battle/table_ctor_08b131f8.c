/* Initialises the 0x2C-byte element table at +0x18 with two entries and the callback at 0x08842954. */
extern void func_08842954(void);
extern void func_08993560(void *table, int count, int size, void (*fn)(void));
void *func_08B131F8(char *self)
{
    func_08993560(self + 0x18, 2, 0x2C, func_08842954);
    return self;
}
