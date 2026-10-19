#include "extra/data_install.h"

extern u32 func_8935828(void *destination, u32 value, u32 size);
extern const char D_089DE758[]; /* data_install_power */
extern const char D_089DE76C[]; /* data_install_fat */
extern const char D_089DE750[]; /* fatms0: */
extern u32 func_089DE69C(u32 first, u32 second, u32 third);
extern u32 func_089DE6C8(u32 first, u32 second, u32 third);
typedef u32 (*DataInstallCallback)(u32, u32, u32);
/* sceKernelCreateCallback / sceKernelDeleteCallback */
extern u32 func_89C5F80(const char *name, DataInstallCallback callback, DataInstallState *state);
extern u32 func_89C5F90(u32 callback);
/* scePowerRegisterCallback / scePowerUnregisterCallback */
extern u32 func_89C57F8(u32 slot, u32 callback);
extern u32 func_89C5810(u32 slot);
/* sceIoDevctl; the callback identifier is passed through a local word. */
extern u32 func_89C5E20(const char *device, u32 command, void *input,
                      u32 input_size, void *output, u32 output_size);

void func_089DD4D0(DataInstallOwner *owner, DataInstallState *state) {
    u32 callback;
    owner->state = state;
    func_8935828(state, 0, 0x45A8);
    owner->state->power_callback = func_89C5F80(D_089DE758, func_089DE69C, owner->state);
    func_89C57F8(2, owner->state->power_callback);
    owner->state->fat_callback = func_89C5F80(D_089DE76C, func_089DE6C8, owner->state);
    callback = owner->state->fat_callback;
    func_89C5E20(D_089DE750, 0x02415821, &callback, 4, 0, 0);
}

void func_089DD584(DataInstallOwner *owner) {
    u32 callback = owner->state->fat_callback;
    func_89C5E20(D_089DE750, 0x02415822, &callback, 4, 0, 0);
    func_89C5F90(owner->state->fat_callback);
    func_89C5810(2);
    func_89C5F90(owner->state->power_callback);
    owner->state = 0;
}
