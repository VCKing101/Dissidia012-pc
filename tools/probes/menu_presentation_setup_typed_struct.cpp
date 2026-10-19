extern "C" {
#define MENU_EXECUTION_STATE_H
#include "menu_presentation_state_view.hpp"
#include "menu/resource_inline.h"
extern void *D_089D7830;
extern void *func_088D0700(void *,int,int,int);
extern void *func_088DAA68(unsigned int);
extern int func_0889F6A0(MainResourceHandlePrefix *,int,unsigned int,unsigned int);
extern void func_0889F750(MainResourceHandlePrefix *,int,int,int,float);
extern int func_088D4C5C(void);
extern void func_08873D34(int,const void *,int,int,int,float);
extern void func_08873DB0(const float *,int);
extern unsigned char D_089DE9D4[];
}
struct PresentationControllerView { unsigned char unknown[8]; void *field_0008; };
struct PresentationFloat4 { float x,y,z,w; const float &first() const { return x; } };
struct PresentationStateData { unsigned char unknown[0x480]; PresentationFloat4 field_0480; };
extern "C" const PresentationStateData D_09B47B50;
extern "C" void func_089DE1D0(MenuRuntimePrefix *self) {
    ((PresentationControllerView *)D_089D7830)->field_0008 = self->field_0138;
    self->field_01DC->field_0014 = (unsigned int)func_088D0700(D_089D7830,0x13,0x4A02,0x4801);
    self->field_01DC->field_0030 = func_088DAA68(self->field_01DC->field_0014);
    self->field_01DC->field_002C = func_088DAA68(self->field_01DC->field_0014);
    self->field_01DC->field_0028 = (unsigned int)func_088DAA40((void *)self->field_01DC->field_0014,0x10,0);
    ((MenuResourceView *)self->field_01DC->field_0028)->field_000E = 0x12;
    self->field_01DC->field_0000 = 0;
    self->field_01DC->field_0001 = 0;
    MainResourceHandlePrefix *handle = &self->subobjects_014C[0].prefix;
    func_0889F6A0(handle,1,(unsigned int)self->field_01DC->field_0058,*(unsigned int *)self->field_01DC->unknown_005C);
    func_0889F750(handle,0,0,0,1.0f);
    func_0889F6A0(&self->subobjects_014C[1].prefix,2,(unsigned int)self->field_01DC->field_0060,*(unsigned int *)self->field_01DC->unknown_0064);
    PresentationFloat4 value = D_09B47B50.field_0480;
    if (!(value.first() < 255.0f)) {
        float ticks = (float)func_088D4C5C();
        func_08873D34(2,D_089DE9D4,0,1,2,ticks);
        float zeros[4] = {0,0,0,0};
        func_08873DB0(zeros,2);
    }
    ((PresentationControllerView *)D_089D7830)->field_0008 = 0;
    self->field_01DC->field_000C = 0;
    self->field_01DC->field_0008++;
}
