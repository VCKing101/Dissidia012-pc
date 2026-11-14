#include "menu/auto_region3.h"

/* Icons72ED0: binds its nodes to their parents and the text to node field_14. */
void func_08A72FB4(Icons72ED0 *self)
{
    LayoutNode *parent;

    parent = &self->field_08;
    func_088D2FB8(parent, &self->field_0C, 7, 3);
    func_088D2FB8(&self->field_00, parent, 0, 1);
    func_088D2FB8(&self->field_04, parent, 0, 2);
    func_088D2FB8(&self->field_00, &self->field_04, 0, 2);
    func_088D3450(&self->field_18, &self->field_14, 0);
}
