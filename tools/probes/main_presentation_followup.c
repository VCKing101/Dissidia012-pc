#include "main_presentation_initialized_view.h"
extern void func_088739CC(MainPresentationChannel *);
extern MainPresentationVector *func_08873DEC(int preset,int index);
void func_08873958(MainPresentationChannel *self,unsigned int kind,const void *first,int second,unsigned char flags,float time) {
    func_0887391C(self,0,flags);
    func_08874350(self,kind,first,second,time);
}
MainPresentationVector *func_08873DEC(int preset,int index) { return func_08873DB0(D_09B48210[preset].values,index); }
void func_08873E14(int index) { func_08873B5C(&D_09B47B50[index]); }
void func_08873E48(void) { func_088739CC(&D_09B47B50[1]); }
int func_08873EB4(void) {
    MainPresentationChannel *channel = D_09B47B50;
    int i = 0;
    do {
        if (channel->field_0234 != 0 || channel->value.values[3] != 0.0f) return 1;
        i++;
        channel++;
    } while (i < 3);
    return 0;
}
int func_08873F10(void) {
    MainPresentationChannel *channel = D_09B47B50;
    int i = 0;
    do {
        if (channel->field_0234 != 0) return 1;
        i++;
        channel++;
    } while (i < 3);
    return 0;
}
