/* Image library: count the child chunks of a GIM chunk (all if id is 0). */
#include "main/sdk_gcc1_types.h"

int func_0899EA2C(SdkGimChunk *chunk, int id)
{
    SdkGimChunk *child;
    SdkGimChunk *end;
    int n;

    if (chunk == 0)
        return 0;
    end = (SdkGimChunk *)((char *)chunk + chunk->next);
    n = 0;
    for (child = (SdkGimChunk *)((char *)chunk + chunk->child); child < end;
         child = (SdkGimChunk *)((char *)child + child->next)) {
        if (id == 0 || child->id == id)
            n++;
    }
    return n;
}

SdkGimChunk *func_0899EA84(SdkGimChunk *chunk, int id, int index)
{
    SdkGimChunk *child;
    SdkGimChunk *end;

    if (chunk == 0)
        return 0;
    end = (SdkGimChunk *)((char *)chunk + chunk->next);
    for (child = (SdkGimChunk *)((char *)chunk + chunk->child); child < end;
         child = (SdkGimChunk *)((char *)child + child->next)) {
        if (id == 0 || child->id == id) {
            if (index-- == 0)
                return child;
        }
    }
    return 0;
}
