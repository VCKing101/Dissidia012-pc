#include "main/task_runtime.h"
extern void func_0885C488(void *);

void func_08882704(MainTaskOwner *owner) {
    void *allocator;
    func_088826B4(owner);
    allocator = owner->field_0000;
    func_0885C538(allocator,owner,0);
    func_0885C488(allocator);
}
