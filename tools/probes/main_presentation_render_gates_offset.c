#include "main_presentation_process_view.h"
extern void func_088741C0(MainPresentationChannel *,const float *);
void func_08874118(MainPresentationChannel *self) {
    if (!(self->value.values[3] <= 0.0f)) func_088741C0(self,self->value.values);
}
void func_0887414C(MainPresentationChannel *self) {
    if (!(self->interpolated_0010.values[3] <= 0.0f)) func_088741C0(self,self->interpolated_0010.values);
}
void func_08874180(MainPresentationChannel *self) {
    int offset = self->field_0220 * sizeof(MainPresentationEvent);
    if (((MainPresentationVector *)((unsigned char *)self+offset))->values[3] != 0.0f)
        func_088741C0(self,(float *)((unsigned char *)self+offset));
}
void func_088740DC(MainPresentationChannel *self) {
    MainPresentationEvent *last = &self->events_0020[self->field_0220-1];
    self->field_0234 = 0;
    if (last->field_0014) last->field_0014(last->field_0018);
}
