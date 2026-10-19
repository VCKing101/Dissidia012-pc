#include "menu/configuration_consumers.h"

void func_08A951F4(void *unused, void **objects, unsigned int index, int override) {
    MenuConfigurationSelection selection;
    selection.index = func_08A93DAC(index);
    if (override != -1) selection.index = override;
    if (selection.index < 0) return;
    selection.first = func_08A93D84(selection.index, 0);
    selection.second = func_08A93D84(selection.index, 1);
    func_088DA0A8(objects[selection.second], selection.first,
                 selection.buffer, &selection.output);
    func_088D9B84(objects[index], selection.buffer);
    func_088D9AF4(objects[index], &selection.output);
}
