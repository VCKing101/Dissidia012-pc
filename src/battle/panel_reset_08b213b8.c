#include "battle/agent27_types.h"

void func_08B213B8(Panel *panel)
{
    panel->active = 0;
    panel->field_004 = 0;
    panel->field_008 = 0;
    panel->field_00C = 0;
    panel->field_010 = 0;
    panel->field_164 = 0;
    panel->field_168 = 0;
    panel->field_16C = 0;
    panel->field_170 = 0;
    panel->field_174 = 0;
    panel->field_178 = 0;
    panel->field_17C = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            panel->field_180[i] = 0;
        }
    }
    panel->field_188 = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            panel->links[i] = 0;
        }
    }
    panel->field_194 = 0;
    panel->field_198 = 0;
    panel->field_19C = 0;
    panel->field_1A0 = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            panel->field_1A4[i] = 0;
            panel->field_1AC[i] = 0;
        }
    }
    panel->field_1B4 = 0;
    panel->field_1B8 = 0;
    panel->field_1BC = 0;
    panel->field_1C0 = 0;
    panel->field_1C4 = 0;
    panel->field_1C8 = 0;
    panel->field_1CC = 0;
    panel->field_1D0 = 0;
}
