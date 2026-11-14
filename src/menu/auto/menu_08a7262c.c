#include "menu/auto_region3.h"

/* Label7259C: sets the text (and its copy) and shows node field_0C once. */
void func_08A7262C(Label7259C *self, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    LayoutNode *node;
    LayoutText *text;
    LayoutText *copy;

    node = &self->field_0C;
    text = &self->field_1C;
    func_088D30E4(node, text);
    copy = &self->field_20;
    func_088D30E4(node, copy);
    func_088D3278(text, arg1, arg2, arg3, arg4, arg5);
    func_088D32F8(copy, text);
    func_088D3374(text, 1, 4);
    func_088D3374(copy, 1, 4);
    self->field_1C.obj->flags = self->field_1C.obj->flags | 0x800;
    func_088D3068(node, text);
    func_088D3068(node, copy);
    if (self->state == 0) {
        func_088D2F68(node, 0);
        func_088D2FA0(node, 2);
        self->state = 1;
    }
}
