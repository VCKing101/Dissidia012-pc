#include "main/drawing_record.h"
extern void func_088D9CFC(MainDrawingRecordPrefix *);
MainDrawingRecordPrefix *func_088DAA68(MainDrawingOwnerPrefix *owner) {
    MainDrawingRecordPrefix *record = func_088DAA40(owner,-1,0);
    func_088D9CFC(record);
    return record;
}
int func_088D4C5C(void) { return 15; }
