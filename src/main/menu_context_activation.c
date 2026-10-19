/* Only this observed flags word is modeled. Full context ownership and bit
 * meanings are unresolved. The incoming value is narrowed to its low byte. */
typedef struct MainMenuContextPrefix {
    unsigned char unknown_0000[0xEF8];
    unsigned int field_0EF8;
} MainMenuContextPrefix;
extern void func_088AC034(void *, unsigned int);
extern void func_088AC07C(void *, unsigned int);

void func_088AC82C(void *object, unsigned char value) {
    MainMenuContextPrefix *self = object;
    unsigned int flags = self->field_0EF8;
    flags |= 4;
    flags |= 2;
    self->field_0EF8 = flags;
    if (value) flags |= 0x40;
    else flags &= ~0x40;
    self->field_0EF8 = flags;
    func_088AC034(self, 8);
    func_088AC07C(self, 1);
}
