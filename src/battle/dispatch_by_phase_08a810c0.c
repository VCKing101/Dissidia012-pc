/* Dispatches to the handler for the entry's phase (1..4); 0 when inactive or out of range. */
typedef struct Ph38 { unsigned char active; char pad[0x13]; int phase; } Ph38;
extern int func_08A8124C(Ph38 *p);
extern int func_08A812DC(Ph38 *p);
extern int func_08A81310(Ph38 *p);
extern int func_08A81420(Ph38 *p);
int func_08A810C0(Ph38 *p)
{
    if (p->active == 0) {
        return 0;
    }
    switch (p->phase) {
    case 1:
        return func_08A8124C(p);
    case 2:
        return func_08A812DC(p);
    case 3:
        return func_08A81310(p);
    case 4:
        return func_08A81420(p);
    }
    return 0;
}
