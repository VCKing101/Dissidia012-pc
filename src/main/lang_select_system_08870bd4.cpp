#include "main/agent33_texture.h"

extern "C" int func_088F49B8(void);   /* system language index */

struct Mlng {
    /* Offset (from the header) of the language's part. */
    static unsigned int offset(D012MlngHeader *mlng, int lang) { return mlng->table[lang]; }
    /* Size of the language's part: the sizes follow the `count` offsets. */
    static unsigned int size(D012MlngHeader *mlng, unsigned int count, int lang) { return mlng->table[count + lang]; }
};

extern "C" {

/* lang_select_system: as lang_select (0x08870B10) but for the system language. */
int func_08870BD4(char *data, void **out_data, int *out_size)
{
    D012MlngHeader *mlng = (D012MlngHeader *)data;

    if (mlng->magic[0] != 'm' || mlng->magic[1] != 'l' || mlng->magic[2] != 'n' || mlng->magic[3] != 'g') {
        return 0;
    }
    int lang = func_088F49B8();
    *out_data = data + Mlng::offset(mlng, lang);
    *out_size = Mlng::size(mlng, mlng->count, lang);
    return 1;
}

}
