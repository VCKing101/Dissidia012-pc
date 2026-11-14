#include "battle/agent27_types.h"

extern void func_088DA0A8(NodeList *list, int index, float *matrix, int *extra);
extern void func_0887B778(ListNode *node, float *matrix);
extern void func_0887B828(ListNode *node, int *extra);

int func_08B28D5C(ModelSlot *slot)
{
    float matrix[16];
    int extra;

    if (slot->list != 0 && slot->node != 0) {
        int index = 0;
        func_088DA0A8(slot->list, index, matrix, &extra);
        func_0887B778(slot->node, matrix);
        func_0887B828(slot->node, &extra);
    }
    return 0;
}
