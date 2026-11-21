#include "menu/agent36_types.h"

/* Finds the current entry's name in its list, prints it into out and
 * returns its value; 0 when there is no entry or no such name. */

extern "C" {
typedef struct NameEntry36 {
    char name[0x12];
    signed char value;        /* +0x12 */
} NameEntry36;

void *func_088A3C98(void *table, int entry);
int func_088A3A48(void *list);
NameEntry36 *func_088A3A68(void *list, int index);
const char *func_08A3C0C4(int id);
int func_08935490(const char *a, const char *b, int size);   /* strncmp */
int func_0893476C(char *buffer, const char *format, ...);    /* sprintf */
}

extern "C" int func_08A3663C(ClockScreen36 *self, char *out)
{
    int entry = self->work->entry;

    if (entry == 0xFF) {
        return 0;
    }
    void *list = func_088A3C98(self->work->table, entry);
    int count = func_088A3A48(list);
    const char *format = func_08A3C0C4(4);
    for (int i = 0; i < count; i++) {
        NameEntry36 *name = func_088A3A68(list, i);
        if (func_08935490(self->work->name, name->name, 0x10) == 0) {
            func_0893476C(out, format, name);
            return name->value;
        }
    }
    return 0;
}
