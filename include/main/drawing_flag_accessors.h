#ifndef MAIN_DRAWING_FLAG_ACCESSORS_H
#define MAIN_DRAWING_FLAG_ACCESSORS_H
#include "main/drawing_record.h"

#ifdef __cplusplus
/* These static inline source forms preserve observed flag access ordering.
 * They introduce no fields or virtual dispatch. Compiler-emitted unused weak
 * bodies are explicitly audited and excluded from recovered-function credit. */
struct MainDrawingFlagAccessors {
    static bool has_flag(const MainDrawingRecordPrefix *self, unsigned int mask) {
        return (self->field_0020 & mask) != 0;
    }
    static void set_flag(MainDrawingRecordPrefix *self, unsigned int mask) { self->field_0020 |= mask; }
    static void clear_flag(MainDrawingRecordPrefix *self, unsigned int mask) { self->field_0020 &= ~mask; }
};
#endif
#endif
