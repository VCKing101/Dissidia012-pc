/* Copies the data into its shared buffer and stores it as link 08, unless that link is already set. */
#include "main/agent17_types.h"

typedef struct Buffer088BCBFC {
    char data[1];
} Buffer088BCBFC;

extern LinkSet *D_089D73B0;
extern Buffer088BCBFC D_09B660D0;

void *func_08935524(void *dst, const void *src, int size);
void func_088BC728(LinkSet *set, void *link);

void func_088BCBFC(const void *src, int size)
{
    if (D_089D73B0->link_08 != 0) {
        return;
    }
    func_08935524(&D_09B660D0, src, size);
    func_088BC728(D_089D73B0, &D_09B660D0);
}
