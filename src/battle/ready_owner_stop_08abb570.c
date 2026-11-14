#include "battle/agent15_types.h"

extern int func_08AC0EB0(void *list);
extern int func_08AB7DD4(void *tasks);

int func_08ABB570(ReadyOwner *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    self->flag_DA = 0;
    func_08AC0EB0(self->list_1B8);
    func_08AB7DD4(self->tasks_1C8);
    return 0;
}
