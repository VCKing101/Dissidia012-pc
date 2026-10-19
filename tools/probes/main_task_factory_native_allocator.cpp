#include "main/task_runtime.h"
struct MainTaskNativeConstruction : MainTask {
    MainTaskNativeConstruction(MainTaskOwner *, unsigned int, int, MainTaskCallback);
    static void *operator new(unsigned int size, void *context) {
        return func_0885C6CC(context,size,0,0,1,0);
    }
};
extern "C" MainTask *func_088823C4(unsigned int owner_word, unsigned int first,
                                 int second, MainTaskCallback update) {
    MainTaskOwner *owner = reinterpret_cast<MainTaskOwner *>(owner_word);
    MainTask *result = new(owner->field_0000) MainTaskNativeConstruction(owner,first,second,update);
    func_08882790(owner,result);
    return result;
}
