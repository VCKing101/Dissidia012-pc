/* Redraws the item panel: item kind 2 with flag 0x20 gets the cursor layout
   and its icon hidden; any other kind gets the plain layout. */
struct Item08AE62AC {
    char pad_00[0xC];
    short field_0C;
    char pad_0E[0x20 - 0xE];
    unsigned short field_20;
};

struct State08AE62AC {
    char pad_000[0x400];
    char field_400[0x600 - 0x400];
    Item08AE62AC *field_600;
    char pad_604[0x880 - 0x604];
    char field_880[4];
};

struct Screen08AE62AC {
    char pad_0000[0xA690];
    State08AE62AC *field_A690;
};

extern "C" {
extern unsigned short D_08AEBBA8[];
extern unsigned short D_08AEBBB8[];
void func_088D442C(char *layout, char *cursor, unsigned short *table);
void func_088D3EA0(State08AE62AC *state, char *layout, unsigned short *table);
void func_088D9F5C(void *part, int id, int show);
void func_088D3F00(State08AE62AC *state, char *layout, unsigned short *table);
void func_088D44EC(char *layout, char *cursor, unsigned short *table);
}

struct Flags08AE62AC {
    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_08AE62AC(Screen08AE62AC *self)
{
    State08AE62AC *state = self->field_A690;
    short kind = state->field_600->field_0C;
    if (kind != -1) {
        if (kind == 2 && Flags08AE62AC::has(state->field_600->field_20, 0x20)) {
            func_088D442C(state->field_400, state->field_880, D_08AEBBB8);
            func_088D3EA0(self->field_A690, self->field_A690->field_400, D_08AEBBA8);
            func_088D9F5C(self->field_A690->field_600, -1, 1);
        } else {
            func_088D3F00(state, state->field_400, D_08AEBBA8);
            func_088D44EC(self->field_A690->field_400, self->field_A690->field_880, D_08AEBBB8);
        }
    }
}
}
