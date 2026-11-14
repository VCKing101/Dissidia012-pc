/* Picks an index at random, each with weight weights[i] + bias (entries
   whose weight is not positive are never picked); -1 when count is 0. */
extern int D_09B6DA54;

int func_089F0274(void *self, short *weights, int count, int bias)
{
    int result = -1;
    int total = 0;
    int roll;
    int i;

    for (i = 0; i < count; i++) {
        if (weights[i] + bias > 0) {
            total = weights[i] + bias + total;
        }
    }
    D_09B6DA54 = D_09B6DA54 * 0x343FD + 0x269EC3;
    roll = ((D_09B6DA54 >> 16) & 0x7FFF) % total + 1;
    for (i = 0; i < count; i++) {
        int weight = weights[i] + bias;
        if (weight > 0) {
            if (weight >= roll) {
                result = (short)i;
                break;
            }
            roll -= weight;
        }
    }
    return result;
}
