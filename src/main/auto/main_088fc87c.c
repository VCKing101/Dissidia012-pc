#include "main/auto_region4.h"

/* Obj8FBBE0: hands text to func_0887BA80 (finished through func_088FCCE0)
 * and counts it. */
void func_088FC87C(Obj8FBBE0 *self, void *text)
{
    int result;

    if (text != 0) {
        result = func_0887BA80(text, func_088FCCE0, 0);
        if (result != 0) {
            self->work->text_count = self->work->text_count + 1;
        }
    }
}
