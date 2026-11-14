/* Copies the data into its shared buffer and stores it as link 04, unless that link is already set. */
#include "main/agent17_types.h"

typedef struct Buffer088BCB94 {
    char data[1];
} Buffer088BCB94;

extern LinkSet *D_089D73B0;
extern Buffer088BCB94 D_09B5DA00;

void *func_08935524(void *dst, const void *src, int size);
void func_088BC714(LinkSet *set, void *link);

void func_088BCB94(const void *src, int size)
{
    if (D_089D73B0->link_04 != 0) {
        return;
    }
    func_08935524(&D_09B5DA00, src, size);
    func_088BC714(D_089D73B0, &D_09B5DA00);
}
