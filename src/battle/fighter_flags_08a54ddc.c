typedef struct Fighter54DDC {
    char pad_000[0x384];
    unsigned int flags;
} Fighter54DDC;

extern void func_08804584(void *self, Fighter54DDC *fighter);

void func_08A54DDC(void *self, Fighter54DDC *fighter)
{
    fighter->flags &= ~0x100;
    fighter->flags &= ~0x200;
    func_08804584(self, fighter);
}
