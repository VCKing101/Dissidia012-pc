#include "battle/auto_region4.h"

void func_08ADC088(void *self, void *arg, int value);

/* Calls func_08ADC088 with the byte the fighter's field_30C points to. */
void func_08ADC328(void *self, void *arg, BattleFighter4 *fighter)
{
    func_08ADC088(self, arg, *fighter->field_30C);
}
