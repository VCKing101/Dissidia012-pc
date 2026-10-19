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
#define main_drawing_has_flag MainDrawingFlagAccessors::has_flag
#define main_drawing_set_flag MainDrawingFlagAccessors::set_flag
#define main_drawing_clear_flag MainDrawingFlagAccessors::clear_flag
