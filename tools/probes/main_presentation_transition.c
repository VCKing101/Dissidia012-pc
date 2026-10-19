#include "main_presentation_view.h"
void func_08874350(MainPresentationChannel *self,unsigned int kind,const void *first,int second,float time) {
    switch (kind) {
    case 0:
        func_08873C20(self,1,0,0,0.0f);
        func_08873C20(self,0,first,second,time);
        break;
    case 1:
        func_08873C20(self,0,0,0,0.0f);
        func_08873C20(self,1,first,second,time);
        break;
    case 2:
        func_08873C20(self,3,0,0,0.0f);
        func_08873C20(self,2,first,second,time);
        break;
    case 3:
        func_08873C20(self,2,0,0,0.0f);
        func_08873C20(self,3,first,second,time);
        break;
    case 4:
        {
            MainPresentationVector value = self->value;
            func_08873B64(self,&value,0,0,0.0f);
            value.w = 0.0f;
            func_08873B64(self,&value,first,second,time);
        }
        break;
    default:
        break;
    }
}
