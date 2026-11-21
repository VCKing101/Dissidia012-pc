#include "menu/agent36_types.h"

/* Closes the page's four parts and the message box unless flag 2 is set,
 * then state 0xC. */

struct Work50EB4 {
    unsigned int flags;       /* +0x0000 */
    int state;                /* +0x0004 */
    char pad_0008[0x6B14];
    void *field_6B1C;
    void *field_6B20;
    char pad_6B24[0x24];
    void *field_6B48;
    char pad_6B4C[0xE4C];
    void *field_7998;
};

struct Screen50EB4 {
    char pad_00[0x58];
    Work50EB4 *work;          /* +0x58 */
};

extern "C" {
void func_08A6B6CC(void *object);
void func_08A6A0F4(void *object);
void func_08A608F0(void *object);
void func_08A6975C(void *object);
void func_088853CC(void *box, unsigned char arg);
}

extern "C" void func_08A50EB4(Screen50EB4 *self)
{
    if (Flags36::Has(self->work->flags, 2)) {
        return;
    }
    func_08A6B6CC(self->work->field_7998);
    func_08A6A0F4(self->work->field_6B48);
    func_08A608F0(self->work->field_6B20);
    func_08A6975C(self->work->field_6B1C);
    int none = 0;
    func_088853CC(D_089D69E8, none);
    self->work->state = 0xC;
}
