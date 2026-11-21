#include "menu/agent36_types.h"

/* Event list screen: closes: builds the visible list, picks the next event
 * name, fades out when one was found, frees the list and starts the
 * event. */

int func_08A3C44C(unsigned short *out, int size);
void func_08A430C0(EventList36 *list);
void func_08A42EF0(char *name, EventList36 *list);
int func_08A42F64(EventList36 *list);
void func_08A42FC0(char *name);
int func_088D4C5C(void);
void func_08873D34(float time, int kind, void *callback, int arg, int one, int two);
void func_08873DEC(int a, int b);
void func_08A3BF2C(void);
void func_08A43628(EventScreen36 *self);

void func_08A43374(EventScreen36 *self)
{
    char name[0x40];

    func_08A3C44C(self->list->ids, 0x200);
    func_08A430C0(self->list);
    func_08A42EF0(name, self->list);
    if (func_08A42F64(self->list) != 0) {
        func_08873D34(func_088D4C5C(), 2, 0, 0, 1, 2);
        func_08873DEC(0, 2);
    }
    func_08A3BF2C();
    func_0885C538(self->heap, self->list->buffer, 0);
    self->list->buffer = 0;
    func_0885C538(self->heap, self->list, 0);
    self->list = 0;
    func_08A43628(self);
    func_08A42FC0(name);
}
