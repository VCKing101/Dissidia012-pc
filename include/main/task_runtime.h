#ifndef MAIN_TASK_RUNTIME_H
#define MAIN_TASK_RUNTIME_H
#include "main/callback_runtime.h"
#include "main/allocator_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MainTaskOwner MainTaskOwner;
typedef void (*MainTaskFinalizer)(MainTask *);
/* Only observed task words are named. Registration links differ from the
 * auxiliary links cleared by construction. Native callback invocation
 * passes the task pointer; lifecycle callbacks remain a separate interface. */
struct MainTask {
    MainTaskCallback field_0000;
    unsigned char unknown_0004[0x20];
    MainTaskOwner *field_0024;
    unsigned int field_0028;
    int field_002C;
    MainTaskFinalizer field_0030;
    MainTask *field_0034;
    MainTask *field_0038;
    MainTask *field_003C;
    MainTask *field_0040;
    unsigned int field_0044;
    unsigned int field_0048;
};
struct MainTaskOwner {
    void *field_0000;
    MainTask *field_0004;
    MainTask *field_0008;
    int field_000C;
    MainTask *field_0010;
    MainTask *field_0014;
};

extern void func_0885C488(void *);
extern MainTask *func_088822BC(MainTask *, MainTaskOwner *, unsigned int,
                                    int, MainTaskCallback);
extern void func_0888245C(MainTaskOwner *, unsigned int, unsigned int);
extern void func_08882790(MainTaskOwner *, MainTask *);
extern void func_088828BC(MainTaskOwner *, MainTask *);
extern void func_088822E0(MainTask *);
extern void func_088825F0(MainTaskOwner *, unsigned int, unsigned int);
extern void func_088826B4(MainTaskOwner *);
extern int func_088826D4(MainTaskOwner *);
extern void func_08882704(MainTaskOwner *);
extern int func_0888274C(MainTaskOwner *, MainTaskCallback);

#ifdef __cplusplus
}
#endif
#endif
