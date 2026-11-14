#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08AAE038[];
extern const unsigned char D_08AAE048[];
extern const unsigned char D_08AAE058[];
}

struct TripleTableObject {
    TableAt08Object base;
    TableAt08Object first;
    TableAt08Object second;

    static void destroy(TripleTableObject *self, unsigned int flags)
    {
        if (self) {
            TableAt08Object::destroy(&self->second, 0, D_08AAE058);
            TableAt08Object::destroy(&self->first, 0, D_08AAE048);
            TableAt08Object::destroy(&self->base, 0, D_08AAE038);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {

void func_08AAB45C(TripleTableObject *self, unsigned int flags)
{
    TripleTableObject::destroy(self, flags);
}

}
