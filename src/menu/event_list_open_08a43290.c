#include "menu/agent36_types.h"

/* Event list screen: opens: allocates the list block and its 0xA000-byte
 * buffer, loads the event file into it and starts the screen task. */

extern void *D_08A4567C;
extern char D_08A45608[];
unsigned int func_08862C40(void *file, int arg);
void func_08862854(void *file, void *buffer, unsigned int size, int arg);
void *func_0885C6CC(void *heap, unsigned int size, int a, int b, int clear, int c);
void func_08A3BE70(void *buffer, unsigned int size);
void func_08A435D0(EventScreen36 *self);
void func_08A436A0(void);
void func_08865088(void *task, int a, int b, void (*step)(void), void *name);

void func_08A43290(EventScreen36 *self)
{
    int one;
    unsigned int size;

    func_08A435D0(self);
    one = 1;
    self->list = func_0885C6CC(self->heap, sizeof(EventList36), 0, 0, one, 0);
    self->list->field_408 = 0;
    size = func_08862C40(D_08A4567C, 1);
    self->list->buffer = func_0885C6CC(self->heap, 0xA000, 0, 0, one, 0);
    func_08862854(D_08A4567C, self->list->buffer, size, 1);
    func_08A3BE70(self->list->buffer, size);
    func_08865088(self->task, 0, 0, func_08A436A0, D_08A45608);
}
