#include "main/agent33_texture.h"

extern "C" int func_08867888(void);   /* current language index */

struct Mlng {
    /* Offset (from the header) of the language's part. */
    static unsigned int offset(D012MlngHeader *mlng, int lang) { return mlng->table[lang]; }
    /* Size of the language's part: the sizes follow the `count` offsets. */
    static unsigned int size(D012MlngHeader *mlng, unsigned int count, int lang) { return mlng->table[count + lang]; }
};

extern "C" {

/* lang_select: if `data` is an "mlng" container, point at the part for the current
 * language. Returns 1 when it was a container (out_data/out_size set), else 0. */
int func_08870B10(char *data, void **out_data, int *out_size)
{
    D012MlngHeader *mlng = (D012MlngHeader *)data;

    if (mlng->magic[0] != 'm' || mlng->magic[1] != 'l' || mlng->magic[2] != 'n' || mlng->magic[3] != 'g') {
        return 0;
    }
    *out_data = data + Mlng::offset(mlng, func_08867888());
    *out_size = Mlng::size(mlng, mlng->count, func_08867888());
    return 1;
}

}
