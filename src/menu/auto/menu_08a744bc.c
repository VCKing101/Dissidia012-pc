#include "menu/auto_region3.h"

/* Btn74390: sets the text. */
void func_08A744BC(Btn74390 *self, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    LayoutText *text;

    text = &self->field_0C;
    func_088D30E4(&self->field_00, text);
    func_088D3278(text, arg1, arg2, arg3, arg4, arg5);
    func_088D3374(text, 1, 4);
    func_088D3068(&self->field_00, text);
}
