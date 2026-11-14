#include "menu/auto_region3.h"

/* NodeText: releases the text. */
void func_08A75CE4(NodeText *self)
{
    func_088D30E4(&self->node, &self->text);
    func_088D3334(&self->text);
}
