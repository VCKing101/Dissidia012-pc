#include "menu/auto_region3.h"

/* Box73E44: binds node field_10 to a layout entry, the other nodes and the texts to it. */
void func_08A73E44(Box73E44 *self, void *source, int index)
{
    LayoutNode *parent;

    parent = &self->field_10;
    func_088D2FB8(parent, source, index, 3);
    func_088D2FB8(&self->field_04, parent, 9, 3);
    func_088D2FB8(&self->field_08, parent, 7, 3);
    func_088D2FB8(&self->field_0C, parent, 3, 3);
    func_088D2FB8(&self->field_00, parent, 0xC, 3);
    func_088D3450(&self->field_14, parent, 8);
    func_088D3450(&self->field_18, parent, 6);
    func_088D3450(&self->field_1C, parent, 5);
    func_088D3450(&self->field_20, parent, 4);
}
