/* Whether the unit is allowed in the current mode (one byte flag per mode). */
typedef struct UnitRules {
    char pad_00[4];
    unsigned char story;
    unsigned char other;
    unsigned char versus;
} UnitRules;

extern unsigned char D_089D5F20;
extern UnitRules *func_088C50D4(void *unit);
extern int func_08867894(void);

int func_088F4554(void *unit)
{
    int allowed = 0;
    UnitRules *rules = func_088C50D4(unit);

    if (D_089D5F20 != 0) {
        if (rules->versus != 0) {
            allowed = 1;
        }
    } else if (func_08867894() != 0) {
        if (rules->story != 0) {
            allowed = 1;
        }
    } else {
        if (rules->other != 0) {
            allowed = 1;
        }
    }
    return allowed;
}
