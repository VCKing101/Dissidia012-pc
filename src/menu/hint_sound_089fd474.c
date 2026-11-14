#include "menu/agent3_types.h"

extern void func_088D4B74(char *sound, int mode);

void func_089FD474(MenuHintOwner *self) {
    MenuHintMarks *marks = &self->field_14C->field_38A0;
    func_088D4B74(marks->field_303CC, 1);
}
