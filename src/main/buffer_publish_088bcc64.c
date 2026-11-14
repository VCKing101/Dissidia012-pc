/* Copies the data into its shared buffer and stores it as link 0C, unless that link is already set. */
#include "main/agent17_types.h"

typedef struct Buffer088BCC64 {
    char data[1];
} Buffer088BCC64;

extern LinkSet *D_089D73B0;
extern Buffer088BCC64 D_09B66900;

void *func_08935524(void *dst, const void *src, int size);
void func_088BC73C(LinkSet *set, void *link);

void func_088BCC64(const void *src, int size)
{
    if (D_089D73B0->link_0C != 0) {
        return;
    }
    func_08935524(&D_09B66900, src, size);
    func_088BC73C(D_089D73B0, &D_09B66900);
}
