#include "menu/auto_region3.h"

/* Btn74390: sets the text from a value. */
void func_08A74454(Btn74390 *self, int value)
{
    LayoutText *text;

    text = &self->field_0C;
    func_088D30E4(&self->field_00, text);
    func_088D321C(text, value);
    func_088D3374(text, 1, 4);
    func_088D3068(&self->field_00, text);
}
