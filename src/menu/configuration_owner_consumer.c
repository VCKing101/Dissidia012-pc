#include "menu/configuration_consumers.h"

void func_08A8D480(MenuConfigurationOwner *self, int index, int override) {
    MenuConfigurationSelection selection;
    selection.index = func_08A93DAC(index);
    if (override != -1) selection.index = override;
    if (selection.index < 0) return;
    selection.first = func_08A93D84(selection.index, 0);
    selection.second = func_08A93D84(selection.index, 1);
    func_088DA0A8(self->field_0008[selection.second + 1], selection.first,
                 selection.buffer, &selection.output);
    func_088D9B84(self->field_0008[index + 1], selection.buffer);
    func_088D9AF4(self->field_0008[index + 1], &selection.output);
}
