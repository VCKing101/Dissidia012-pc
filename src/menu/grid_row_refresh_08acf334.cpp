/* Menu grid refresh: updates the current cell, then re-runs the cell updater for every cell of the selected row whose record has the given field. */
struct Slot { short id; char p[4]; unsigned char flags; char q; };
struct Row { Slot s[150]; };
struct W { char p0[0x1224]; int sel; char p1[0x1430 - 0x1228]; int cnt[9]; char p2[0x148A - 0x1454]; Row rows[9]; };
struct Rec { short id; char p[4]; short v; };
struct Self { char p[0xDB30]; W *w; };
extern "C" {
int func_08AF96B0(void);
Rec *func_088B1D9C(int);
void func_08ACF284(Self *, int);
void func_08ACF334(Self *self, short v)
{
    int c;
    if (func_08AF96B0() != 0) {
        return;
    } else {
        func_08ACF284(self, v);
        for (c = 0; c < self->w->cnt[self->w->sel]; c++) {
            Rec *rec = func_088B1D9C(self->w->rows[self->w->sel].s[c].id);
            if (rec != 0) {
                if (rec->v == v) {
                    func_08ACF284(self, rec->id);
                }
            }
        }
    }
}
}
