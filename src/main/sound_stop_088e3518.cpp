/* Stops the sound manager and releases the handle at 0x670. */
#include "main/agent19_types.h"

struct SoundUser {
    char pad_000[0x670];
    void *handle;
};

extern "C" {
extern void *D_089D6998;
extern void func_0887E684(void *manager);
extern void func_088665C4(void *handle);

void func_088E3518(SoundUser *user)
{
    if (user->handle == 0) {
        return;
    }
    Agent19Kept<SoundUser *> saved(user);
    func_0887E684(D_089D6998);
    func_088665C4(saved.value->handle);
    saved.value->handle = 0;
}
}
