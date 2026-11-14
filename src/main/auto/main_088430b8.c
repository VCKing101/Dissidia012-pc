#include "main/auto_region2.h"

/* Takes the record for id, copies D_089C8850 into its field_34, then applies up to three argument pairs. */
void func_088430B8(Obj8430B8 *self, int id, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8)
{
    Record9D4968 *record;

    record = func_088470B8(id);
    self->record = record;
    func_08844FA4(record->field_34, &D_089C8850);
    func_0884333C(self, arg3, arg4);
    if ((arg5 != 0) && (arg6 != 0)) {
        func_088459E0(self, arg5, arg6);
    }
    if ((arg7 != 0) && (arg8 != 0)) {
        func_08843460(self, arg7, arg8);
    }
}
