#include "main/agent33_texture.h"

extern "C" {

extern D012ModelBank *D_089D5F94;

/* model_holder_build: split the file into GMOs and build one model per part. */
void func_0886DFC0(D012ModelHolder *holder, const char *name, void *data, int size);

/* model_holder_new: take the first free model holder and build a model file in memory into it.
 * Returns the holder, or 0 when all 0xC0 are in use. */
D012ModelHolder *func_0886D2C4(const char *name, void *data, int size)
{
    D012ModelHolder *holder = D_089D5F94->holders;

    for (int i = 0; i < D012_MODEL_HOLDERS; i++, holder++) {
        if (!D012ModelPart::loaded(holder->parts)) {
            func_0886DFC0(holder, name, data, size);
            return holder;
        }
    }
    return 0;
}

}
