/* Reduces the float timer of entry idx by func_08B11760; returns 1 when it was already zero or less. */
extern float func_08B11760(void);
typedef struct { int a; float t; } Entry;
int func_08B0F0AC(Entry *entries, int idx) {
    float *p = &entries[idx].t;
    float zero = 0.0f;
    float v = *p;
    if (v > zero) {
        float d = func_08B11760();
        *p = *p - d;
        return 0;
    }
    return 1;
}
