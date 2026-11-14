#include "main/auto_region4.h"

/* Releases the three entries of D_089D7F00 from its player at +0x1C, then the player. */
void func_088EDA0C(void)
{
    void *player;

    player = D_089D7F00->field_1C;
    if (D_089D7F00->field_A0 != 0) {
        func_089C5B28(player, D_089D7F00->field_A0);
        D_089D7F00->field_A0 = 0;
        player = D_089D7F00->field_1C;
    }
    if (D_089D7F00->field_A4 != 0) {
        func_089C5B28(player, D_089D7F00->field_A4);
        D_089D7F00->field_A4 = 0;
        player = D_089D7F00->field_1C;
    }
    if (D_089D7F00->field_A8 != 0) {
        func_089C5B28(player, D_089D7F00->field_A8);
        D_089D7F00->field_A8 = 0;
        player = D_089D7F00->field_1C;
    }
    func_089C5B20(player);
}
