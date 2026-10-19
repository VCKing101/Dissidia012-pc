#include "main/presentation_channel.h"
extern void func_088741C0(MainPresentationChannel *,const float *);
void func_08874118(MainPresentationChannel *self) {
    if (!(self->value.values[3] <= 0.0f)) func_088741C0(self,self->value.values);
}
void func_0887414C(MainPresentationChannel *self) {
    if (!(self->interpolated_0010.values[3] <= 0.0f)) func_088741C0(self,self->interpolated_0010.values);
}
