/* Rolls the rules for `value`: an enabled rule (+0x10) whose count is
   below its maximum and whose window [+0x08, +0x10] holds the value hits
   with chance 1 / (slots left + 1); a hit counts up. Returns 1 on a hit.
   The rules are parallel arrays: max[4], low[4], high[4]. */
extern int D_09B6DA54;

int func_089F07E0(void *self, unsigned short value, unsigned short *rule, unsigned short *counts)
{
    int result = 0;
    int i;

    for (i = 0; i < 4; i++) {
        if (rule[i] == 0xFFFF) {
            break;
        }
        if (rule[i + 8] != 0) {
            if (counts[i] < rule[i]) {
                if (rule[i + 4] <= value && value <= rule[i + 8]) {
                    int left = (rule[i + 8] - value) / (rule[i] - counts[i]);
                    if (left >= 0) {
                        unsigned short roll;
                        D_09B6DA54 = D_09B6DA54 * 0x343FD + 0x269EC3;
                        roll = ((D_09B6DA54 >> 16) & 0x7FFF) % (left + 1);
                        if (roll == 0) {
                            counts[i]++;
                            result = 1;
                        }
                        break;
                    }
                }
            }
        }
    }
    return result;
}
