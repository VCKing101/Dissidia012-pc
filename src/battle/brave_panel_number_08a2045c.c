#include "battle/agent28_types.h"

/* Shows the side's Brave on the panel, clamped to 0..9999. */
extern RootHolder *D_089D6A10;
extern int func_089DDBA8(void *root, unsigned int side);
extern int func_089E9964(unsigned int side);
extern void func_089F7EB8(void *parts, int value, int color);

int func_08A2045C(BravePanel *panel)
{
    void *root = D_089D6A10->root;
    int brave;
    int color;

    if (root == 0) {
        return -2;
    }
    brave = func_089DDBA8(root, panel->side);
    if (brave < 0) {
        brave = 0;
    } else if (brave >= 10000) {
        brave = 9999;
    }
    color = func_089E9964(panel->side);
    func_089F7EB8(panel->parts, brave, color);
    return 0;
}
