#include "main/sdk_gcc2_types.h"

/* Stream reader state on the stack (0x70 bytes). */
typedef struct SdkStream {
    char data[0x70];
} SdkStream;

extern void func_089AB7A0(SdkStream *s);
extern void func_089ACEC0(int count, SdkStream *s);
extern int func_089ABD28(SdkStream *s);
extern void *func_089AD080(int count, SdkStream *s);
extern void func_089ABCA0(SdkStream *s);

void *func_089AD0A0(int count)
{
    SdkStream s;
    void *p;

    func_089AB7A0(&s);
    func_089ACEC0(count, &s);
    if (func_089ABD28(&s) == 0) {
        return 0;
    }
    p = func_089AD080(count, &s);
    func_089ABCA0(&s);
    return p;
}
