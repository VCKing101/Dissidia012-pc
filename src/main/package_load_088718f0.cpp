#include "main/agent7_types.h"

struct Package {
    int handle;
    Agent7Manager *manager;
};

extern "C" {

extern const char *D_089D5FEC;
extern const char *D_089D5FE8;
extern char D_09B26BC0[];

void func_088718F0(Package *self)
{
    self->manager->call29(self->handle);
    self->handle = -1;
}

void func_08871938(Package *self)
{
    self->manager->load04(D_089D5FEC, D_09B26BC0, 0x20F8C);
    self->handle = self->manager->open22(D_089D5FE8);
}

}
