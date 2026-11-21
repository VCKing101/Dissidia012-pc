#include "menu/agent36_types.h"

struct Work46FB4 {
    char pad_000[0x410];
    void *table;              /* +0x410 */
};

struct Screen46FB4 {
    char pad_000[0x418];
    Work46FB4 *work;          /* +0x418 */
};

extern "C" {
extern int D_09B6DABC;
extern unsigned short D_08A4810A;
int func_08A484D8(void *table, int column, int row);
void func_08A4639C(Screen46FB4 *self, int id);
}

/* The menu's random generator (MSVC rand() constants), inlined. */
struct Random36 {
    static int Next()
    {
        D_09B6DABC = D_09B6DABC * 0x343FD + 0x269EC3;
        return (D_09B6DABC >> 16) & 0x7FFF;
    }
};

extern "C" void func_08A46FB4(Screen46FB4 *self, int row)
{
    int voice[1];
    int flags[1];
    int rare_voice[2];

    voice[0] = func_08A484D8(self->work->table, 4, row);
    if (voice[0] == 0xFFFF) {
        func_08A4639C(self, D_08A4810A);
    } else {
        int rare = 0;

        if (Random36::Next() % 100 < 25) {
            rare = 1;
        }

        flags[0] = func_08A484D8(self->work->table, 0, row);
        if (flags[0] & 4) {
            if (flags[0] & 8) {
                rare_voice[0] = func_08A484D8(self->work->table, 5, row);
                if (rare_voice[0] != 0xFFFF) {
                    if (rare == 1) {
                        func_08A4639C(self, rare_voice[0]);
                    } else {
                        func_08A4639C(self, voice[0]);
                    }
                } else {
                    func_08A4639C(self, voice[0]);
                }
            } else {
                func_08A4639C(self, voice[0]);
            }
        } else {
            func_08A4639C(self, D_08A4810A);
        }
    }
}
