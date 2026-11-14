#include "main/agent7_types.h"

struct PackageRequest {
    int handle;
    Agent7Manager *manager;
    char pad_08[0x58];
    int field_60;
    int field_64;
};

extern "C" {

void func_08871AD4(void *ctx);

void func_088719B8(PackageRequest *self)
{
    self->manager->request08(self->handle, self->field_60, self->field_64, func_08871AD4, self, 0);
}

}
