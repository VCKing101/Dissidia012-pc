#include "menu/agent36_types.h"

/* Choice screen: acts on the chosen option: 0 sends arg to the session
 * (when there is one), 1 starts the entry or the link's request. */

extern void *D_089D6C30;
extern void *D_089D6C88;
int func_0889F280(void *net);
void func_0889F264(void *net, int arg);
void func_088A1758(void *object, int id);
void func_0889F830(void *block, int arg);

void func_08A37558(ClockScreen36 *self, int choice, int arg)
{
    ClockWork36 *work;
    int id;

    switch (choice) {
    case 0:
        if (self->work->session != -1) {
            if (func_0889F280(D_089D6C30) != 0) {
                func_0889F264(D_089D6C30, arg);
            }
        }
        self->work->field_D88 = -1;
        break;
    case 1:
        work = self->work;
        if (work->field_D78 == 1) {
            id = work->field_D5C;
            if (id != -1) {
                func_088A1758(D_089D6C88, id);
            }
        } else {
            func_0889F830(work->link->block, 10);
        }
        break;
    }
}
