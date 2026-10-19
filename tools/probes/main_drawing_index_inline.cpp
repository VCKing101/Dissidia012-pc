#include "main/drawing_flag_accessors.h"
extern "C" void func_088D9A94(MainDrawingRecordPrefix *);
extern "C" void func_088DAC1C(void *, int);
struct MainDrawingIndexReset {
    static void refresh(MainDrawingRecordPrefix *self, int index) {
        self->field_000C = index;
        self->field_000E = -1;
        self->field_0022 = -1;
        func_088D9A94(self);
        unsigned short kept = self->field_0020 & 0x2000;
        self->field_0008 = 1.0f;
        self->field_0020 = kept;
        self->field_0004 = -1.0f;
        MainDrawingFlagAccessors::set_flag(self, 0x1A);
        func_088D9D24(self);
        func_088DAC1C(self->field_0024, self->field_000C);
    }
};
extern "C" void func_088D9F5C(MainDrawingRecordPrefix *self, int index, unsigned char force) {
    if (index != -1) {
        if (self->field_000C != index) MainDrawingIndexReset::refresh(self,index);
        else if (force) MainDrawingIndexReset::refresh(self,index);
    } else {
        self->field_000C = -1;
        MainDrawingFlagAccessors::clear_flag(self,8);
        func_088D9D24(self);
    }
}
