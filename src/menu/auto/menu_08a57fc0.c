#include "menu/auto_region2.h"

/* Panel58420: reads the six items back into its record. */
void func_08A57FC0(Panel58420 *self)
{
    Rec164 *record;

    record = *self->record;
    func_08A566B8(&self->field_100, &record->field_08, &record->field_09, &record->field_0A);
    record = *self->record;
    func_08A566B8(&self->field_33C, &record->field_0C, &record->field_0D, &record->field_0E);
    func_08A55B4C(&self->field_578, &(*self->record)->field_06);
    func_08A56008(&self->field_AB0, &(*self->record)->field_00);
    func_08A56B6C(&self->field_DFC, &(*self->record)->field_02);
    func_08A56E30(&self->field_F54, &(*self->record)->field_04, (*self->record)->field_10);
}
