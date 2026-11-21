#include "main/agent33_texture.h"

/* The graphics object (only the colour-mode flag is read here). */
struct Gfx0886A6A4 {
    unsigned char unk_0000[0x2CCF];
    unsigned char color32;             /* +0x2CCF */
};

struct BuildFlags {
    static bool has(unsigned int flags, unsigned int bit) { return (flags & bit) != 0; }
};

extern "C" {

extern Gfx0886A6A4 *D_089D5EC0;
int func_089B0720(void *model, int pass);     /* gmo_build: 1 = done */
void func_089B0BAC(void *model);              /* undo a failed build */

/* model_lazy_build: build a holder's model once, on first use. Flag 4 builds the parts
 * (materials, lists), flag 8 copies the model's textures into VRAM (gmo_build -> gmo_tex_place);
 * the alternate path (flag 0x10) also runs passes 1 and 2, and is skipped in 32-bit colour
 * mode. Any failed pass is undone and the holder stays unbuilt. */
void func_0886A6A4(D012ModelHolder *holder)
{
    if (holder->unk_88 != 0) {
        return;
    }
    unsigned int flags = holder->flags;
    if (BuildFlags::has(flags, D012_MODEL_BUILD_TRIED)) {
        return;
    }
    flags |= D012_MODEL_BUILD_TRIED;
    holder->flags = flags;
    if (BuildFlags::has(flags, D012_MODEL_BUILT)) {
        return;
    }
    if (BuildFlags::has(flags, D012_MODEL_BUILD_ALT)) {
        if (D_089D5EC0->color32 == 0) {
            if (BuildFlags::has(flags, D012_MODEL_BUILD_PARTS) && !func_089B0720(holder->current_part_addr->model_addr, D012_MODEL_BUILD_PARTS)) {
                func_089B0BAC(holder->current_part_addr->model_addr);
                return;
            }
            if (!func_089B0720(holder->current_part_addr->model_addr, D012_MODEL_BUILD_PASS1)) {
                func_089B0BAC(holder->current_part_addr->model_addr);
                return;
            }
            if (BuildFlags::has(holder->flags, D012_MODEL_BUILD_VRAM) && !func_089B0720(holder->current_part_addr->model_addr, D012_MODEL_BUILD_VRAM)) {
                func_089B0BAC(holder->current_part_addr->model_addr);
                return;
            }
            if (!func_089B0720(holder->current_part_addr->model_addr, D012_MODEL_BUILD_PASS2)) {
                func_089B0BAC(holder->current_part_addr->model_addr);
                return;
            }
            holder->flags |= D012_MODEL_BUILT;
        }
    } else {
        if (BuildFlags::has(flags, D012_MODEL_BUILD_PARTS) && !func_089B0720(holder->current_part_addr->model_addr, D012_MODEL_BUILD_PARTS)) {
            func_089B0BAC(holder->current_part_addr->model_addr);
            return;
        }
        if (BuildFlags::has(holder->flags, D012_MODEL_BUILD_VRAM)) {
            if (!func_089B0720(holder->current_part_addr->model_addr, D012_MODEL_BUILD_VRAM)) {
                func_089B0BAC(holder->current_part_addr->model_addr);
                return;
            }
        }
        holder->flags |= D012_MODEL_BUILT;
    }
}

}
