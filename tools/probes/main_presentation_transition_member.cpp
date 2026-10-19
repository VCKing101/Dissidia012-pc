#include "presentation_member_transition.h"
void MainPresentationChannel::func_08874350(unsigned int kind,const void *first,int second,float time) {
    switch (kind) {
    case 0:
        func_08873C20(this,1,0,0,0.0f);
        func_08873C20(this,0,first,second,time);
        break;
    case 1:
        func_08873C20(this,0,0,0,0.0f);
        func_08873C20(this,1,first,second,time);
        break;
    case 2:
        func_08873C20(this,3,0,0,0.0f);
        func_08873C20(this,2,first,second,time);
        break;
    case 3:
        func_08873C20(this,2,0,0,0.0f);
        func_08873C20(this,3,first,second,time);
        break;
    case 4:
        {
            MainPresentationVector value = {{this->value.values[0],this->value.values[1],this->value.values[2],this->value.values[3]}};
            func_08873B64(this,value.values,0,0,0.0f);
            value.values[3] = 0.0f;
            func_08873B64(this,value.values,first,second,time);
        }
        break;
    }
}
