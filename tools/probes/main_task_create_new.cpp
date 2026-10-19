#include "main/task_runtime.h"

struct MainTaskConstruction : MainTask {
    static void *operator new(unsigned int size, void *allocator) {
        return func_0885C6CC(allocator,size,0,0,1,0);
    }
    MainTaskConstruction(MainTaskOwner *owner, const unsigned int &first, const int &second,
                         MainTaskCallback const &update) {
        func_088822BC(this,owner,first,second,update);
    }
};

extern "C" MainTask *func_088823C4(unsigned int owner_word, unsigned int first,
                                 int second, MainTaskCallback update) {
    MainTaskOwner *owner = reinterpret_cast<MainTaskOwner *>(owner_word);
    MainTask *result = new(owner->field_0000) MainTaskConstruction(owner,first,second,update);
    func_08882790(owner,result);
    return result;
}
