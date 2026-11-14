#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08AAE0A8[];
extern const unsigned char D_08AAE0B8[];
extern const unsigned char D_08AAE0C8[];
}

struct TripleTableObject {
    TableAt08Object base;
    TableAt08Object first;
    TableAt08Object second;

    static void destroy(TripleTableObject *self, unsigned int flags)
    {
        if (self) {
            TableAt08Object::destroy(&self->second, 0, D_08AAE0C8);
            TableAt08Object::destroy(&self->first, 0, D_08AAE0B8);
            TableAt08Object::destroy(&self->base, 0, D_08AAE0A8);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {

void func_08AAB768(TripleTableObject *self, unsigned int flags)
{
    TripleTableObject::destroy(self, flags);
}

}
