#include "main_presentation_view.h"
MainPresentationVector *func_08873B18(MainPresentationVector *self,const MainPresentationVector *source) { return func_089A3868(self,source); }
void func_08873B34(MainPresentationVector *self,int index) { func_08873B18(self,&D_09B48210[index]); }
void func_08873B5C(MainPresentationChannel *self) { self->field_0234 = 0; }
int func_08873B64(MainPresentationChannel *self,const MainPresentationVector *value,const void *first,int second,float time) {
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
int func_08873C20(MainPresentationChannel *self,int index,const void *first,int second,float time) { return func_08873B64(self,&D_09B48210[index],first,second,time); }
MainPresentationChannel *func_08873CE0(int first,unsigned char second,int index) {
    MainPresentationChannel *channel = &D_09B47B50[index];
    func_0887391C(channel,first,second);
    return channel;
}
MainPresentationChannel *func_08873D34(unsigned int first,const void *second,int third,unsigned char fourth,int index,float time) {
    MainPresentationChannel *channel = func_08873CE0(0,fourth,index);
    func_08874350(channel,first,second,third,time);
    return channel;
}
void func_08873DB0(const MainPresentationVector *value,int index) { func_08873B18(&D_09B47B50[index].value,value); }
