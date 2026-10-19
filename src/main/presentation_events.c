#include "main/presentation_channel.h"
MainPresentationVector *func_08873B18(MainPresentationVector *self,const float *source) { return func_089A3868(self,source); }
MainPresentationVector *func_08873B34(MainPresentationVector *self,int index) { return func_08873B18(self,D_09B48210[index].values); }
void func_08873B5C(MainPresentationChannel *self) { self->field_0234 = 0; }
int func_08873B64(MainPresentationChannel *self,const float *value,MainPresentationCallback first,int second,float time) {
    if (self->field_0220 < 16) {
        if (time <= self->field_022C) return 0;
    } else { return 0; }
    func_089A3868(&self->events_0020[self->field_0220].value,value);
    self->events_0020[self->field_0220].field_0010 = time;
    self->events_0020[self->field_0220].field_0014 = first;
    self->events_0020[self->field_0220].field_0018 = second;
    self->field_0220++;
    self->field_022C = time;
    return 1;
}
int func_08873C20(MainPresentationChannel *self,int index,MainPresentationCallback first,int second,float time) { return func_08873B64(self,D_09B48210[index].values,first,second,time); }
