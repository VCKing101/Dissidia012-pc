#include "battle/agent15_types.h"

extern int func_08AC0F28(void *list);
extern int func_08AB7E50(void *tasks);

int func_08ABB5BC(ReadyOwner *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    self->flag_DA = 1;
    func_08AC0F28(self->list_1B8);
    func_08AB7E50(self->tasks_1C8);
    return 0;
}
