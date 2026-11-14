typedef struct HealthPair {
    short current;
    short max;
} HealthPair;

/* Danger level of side's health: 0 above half, 1 above 30%, 2 at or below 30%. */
int func_08A97934(void *self, HealthPair *pairs, unsigned int side)
{
    HealthPair *pair;
    float ratio;
    if (pairs == 0 || side >= 2) {
        return 0;
    }
    pair = &pairs[side];
    if (pair->max == 0) {
        return 0;
    }
    ratio = (float)pair->current / (float)pair->max;
    if (ratio > 0.5f) {
        return 0;
    }
    if (ratio > 0.3f) {
        return 1;
    }
    return 2;
}
