#include "menu/agent36_types.h"

/* Constructor of the clock / choice screen: base, vtable, its entries and
 * the work block on the battle table's layout. */

extern "C" {
extern char D_08A38EA8[];
void func_08A408A0(void *self);
void func_08993560(void *array, int count, int size, void (*ctor)(void));  /* array constructor */
void func_088A3900(void);
}

/* The clock / choice screen object (vtable D_08A38EA8). */
struct ClockObject36 {
    void *vtable;
    char block[0xDBC];        /* +0x004: the work block */
    char entries[0x23][0xC];  /* +0xDC0 */
    ClockWork36 *work;        /* +0xF64 */
};

extern "C" void func_08A365D4(ClockObject36 *self);

extern "C" ClockObject36 *func_08A37890(ClockObject36 *self)
{
    int i;

    func_08A408A0(self);
    self->vtable = D_08A38EA8;
    func_08993560(self->entries, 0x23, 0xC, func_088A3900);
    self->work = (ClockWork36 *)self->block;
    self->work->layout = D_08A457EC->table;
    self->work->handles = D_08A457EC->table->handles;
    self->work->messages = D_08A457EC->table->messages;
    for (i = 0; i < 0x17; i++) {
        self->work->texts[i] = 0;
    }
    func_08A365D4(self);
    self->work->script = 0;
    self->work->field_120 = 0;
    self->work->field_DB4 = 0;
    self->work->field_DB5 = 0;
    self->work->field_DB6 = 0;
    self->work->field_DB7 = 0;
    self->work->field_D94 = 0;
    self->work->state = 0;
    return self;
}
