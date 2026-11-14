/* Copies the data into its shared buffer and stores it as link 00, unless that link is already set. */
#include "main/agent17_types.h"

typedef struct Buffer088BCB2C {
    char data[1];
} Buffer088BCB2C;

extern LinkSet *D_089D73B0;
extern Buffer088BCB2C D_09B59AF8;

void *func_08935524(void *dst, const void *src, int size);
void func_088BC700(LinkSet *set, void *link);

void func_088BCB2C(const void *src, int size)
{
    if (D_089D73B0->link_00 != 0) {
        return;
    }
    func_08935524(&D_09B59AF8, src, size);
    func_088BC700(D_089D73B0, &D_09B59AF8);
}
