struct Totals { int gil; int kills; int wins;
    static int clamp(int v) { if (v > 999999999) { v = 999999999; } return v; } };
extern "C" {
void func_088B2B24(Totals *t, Totals *add)
{
    t->gil = Totals::clamp(t->gil + add->gil);
    t->kills = Totals::clamp(t->kills + add->kills);
    t->wins = Totals::clamp(t->wins + add->wins);
}
}
