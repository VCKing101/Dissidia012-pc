#include "battle/agent28_types.h"

/* Loads the Brave panel's model for its side, then binds the parts and plays
 * the panel's cue. */
extern "C" {

extern unsigned int func_08A22CA8(unsigned int side);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_089F7E18(void *parts);
extern void func_08889F28(unsigned int side, int cue);
extern void func_08A202DC(BravePanel *panel);
extern int D_08A20678[2];

int func_08A20328(BravePanel *panel)
{
    if (!panel->open) {
        return -1;
    }
    if (panel->model == 0) {
        unsigned int kind = func_08A22CA8(panel->side);
        if (kind >= 2) {
            return -2;
        }
        panel->model = func_088DAA40(panel->source, D_08A20678[kind], 0);
        if (panel->model == 0) {
            return -3;
        }
        func_089F7E18(panel->parts);
        func_08889F28(panel->side, panel->cue + 5);
        func_08A202DC(panel);
    }
    return 0;
}

}
