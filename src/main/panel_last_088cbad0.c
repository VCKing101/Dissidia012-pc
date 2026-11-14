/* Last value of the current panel, or the owner's fallback when that is 0. */
typedef struct Panel088CBAD0 {
    char data[0xC5];
    unsigned char last;
} Panel088CBAD0;

typedef struct Owner088CBAD0 {
    char field_00;
    unsigned char fallback;
    char pad_02[0xE];
    Panel088CBAD0 panels[9];
    char pad_706[0x773 - 0x10 - 9 * 0xC6];
    signed char current;
} Owner088CBAD0;

unsigned char func_088CBAD0(Owner088CBAD0 *o)
{
    Panel088CBAD0 *panels = o->panels;
    unsigned char v = panels[o->current].last;
    if (v > 0) {
        return v;
    }
    return o->fallback;
}
