#include "main/presentation_channel.h"
MainPresentationChannel *func_08873CE0(int first,unsigned char second,int index) {
    MainPresentationChannel *channel = &D_09B47B50[index];
    func_0887391C(channel,first,second);
    return channel;
}
MainPresentationChannel *func_08873D34(unsigned int first,MainPresentationCallback second,int third,unsigned char fourth,int index,float time) {
    MainPresentationChannel *channel = func_08873CE0(0,fourth,index);
    func_08874350(channel,first,second,third,time);
    return channel;
}

