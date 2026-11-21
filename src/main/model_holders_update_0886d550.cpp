#include "main/agent33_texture.h"

extern "C" {

extern D012ModelBank *D_089D5F94;

/* model_lazy_build: build a holder's model passes (once; textures to VRAM on flag 8). */
void func_0886A6A4(D012ModelHolder *holder);

/* model_holders_update: run the lazy build on every holder in use. */
void func_0886D550(void)
{
    int i = 0;

    for (D012ModelHolder *holder = &D_089D5F94->holders[i]; i < D012_MODEL_HOLDERS; i++, holder++) {
        if (D012ModelPart::loaded(holder->parts)) {
            func_0886A6A4(holder);
        }
    }
}

}
