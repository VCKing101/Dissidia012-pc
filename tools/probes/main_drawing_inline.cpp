/* Probe only: original-style inline flag accessors under the pinned profile. */
extern "C" {
#include "main/drawing_record.h"
}

struct MainDrawingFlagAccessors {
    static bool has_flag(const MainDrawingRecordPrefix *self, unsigned int mask) {
        return (self->field_0020 & mask) != 0;
    }
    static void set_flag(MainDrawingRecordPrefix *self, unsigned int mask) { self->field_0020 |= mask; }
    static void clear_flag(MainDrawingRecordPrefix *self, unsigned int mask) { self->field_0020 &= ~mask; }
};

extern "C" void func_088D9D24(MainDrawingRecordPrefix *self) {
    if (self->field_000C != -1 && self->field_0028[3] && MainDrawingFlagAccessors::has_flag(self,8) &&
        !(unsigned char)((self->field_0020 & 0x4000) != 0)) {
        MainDrawingFlagAccessors::set_flag(self,0x8000);
    } else MainDrawingFlagAccessors::clear_flag(self,0x8000);
    if (self->field_000C != -1 && MainDrawingFlagAccessors::has_flag(self,8) &&
        !(unsigned char)((self->field_0020 & 0x20) != 0)) {
        MainDrawingFlagAccessors::set_flag(self,1);
    } else MainDrawingFlagAccessors::clear_flag(self,1);
}

extern "C" void func_088D9DDC(MainDrawingRecordPrefix *self) {
    if (self->field_0028[0] == 0x80 && self->field_0028[1] == 0x80 &&
        self->field_0028[2] == 0x80 && self->field_0028[3] == 0xFF) {
        MainDrawingFlagAccessors::set_flag(self,0x200);
    } else MainDrawingFlagAccessors::clear_flag(self,0x200);
    func_088D9D24(self);
}
