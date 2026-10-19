#include "main/resource_manager_commands.h"

int func_088A1638(MainManagerSlotPrefix *self, int index) {
    MainManagerSlotPrefix *slot = (MainManagerSlotPrefix *)((unsigned char *)self + index);
    if (slot->field_0068[0] == 1) {
        slot->field_0068[0] = 0;
        return func_089907F8(index);
    }
    return 0;
}

int func_088A1678(void *manager, int id) { return func_08990CA0(id); }
int func_088A1694(void *manager, int id) { return func_08990DC8(id); }
