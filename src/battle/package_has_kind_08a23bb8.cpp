#include "battle/agent31_types.h"

extern "C" int func_088916A4(A31Package *package, A31PackageInfo *info);
extern "C" int func_08891894(A31Package *package, A31PackageEntry *entry, int index);

/* True when the package holds an entry of kind 2. */
extern "C" int func_08A23BB8(ReadyScreen *self, int index)
{
    A31Package *package = &self->packages[index];
    A31PackageInfo info;
    A31PackageEntry entry;
    if (func_088916A4(package, &info) != 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < info.count; i++) {
        int r = func_08891894(package, &entry, info.first + i);
        if (r != 0) {
            continue;
        }
        int kind = entry.kind;
        if (kind != 2) {
            continue;
        }
        found = 1;
        break;
    }
    return found;
}
