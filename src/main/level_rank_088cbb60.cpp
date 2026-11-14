/* Rank of the level against the descending limit table (0..10). */
struct Stats088CBB60 {
    char field_00;
    char field_01;
    unsigned char level;
};
extern "C" {
extern short D_089CBCC8[11];
unsigned int func_088CBB60(Stats088CBB60 *s)
{
    for (unsigned int i = 0; i < 11; i++) {
        if (D_089CBCC8[i] <= s->level) {
            return i;
        }
    }
    return 10;
}
}
