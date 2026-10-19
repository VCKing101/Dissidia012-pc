#include "main_task_iteration.hpp"
extern "C" void func_088825F0(MainTaskOwner *owner, unsigned int mask, unsigned int value) {
    MainTask *task = MainTaskIteration::matching_next(owner,0,mask,value);
    while (task) {
        MainTask *next = MainTaskIteration::matching_next(owner,task,mask,value);
        func_088828BC(owner,task);
        task = next;
    }
}
