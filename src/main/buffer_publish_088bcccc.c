/* Copies the data into its shared buffer and stores it as link 10, unless that link is already set. */
#include "main/agent17_types.h"

typedef struct Buffer088BCCCC {
    char data[1];
} Buffer088BCCCC;

extern LinkSet *D_089D73B0;
extern Buffer088BCCCC D_09B673A0;

void *func_08935524(void *dst, const void *src, int size);
void func_088BC750(LinkSet *set, void *link);

void func_088BCCCC(const void *src, int size)
{
    if (D_089D73B0->link_10 != 0) {
        return;
    }
    func_08935524(&D_09B673A0, src, size);
    func_088BC750(D_089D73B0, &D_09B673A0);
}
