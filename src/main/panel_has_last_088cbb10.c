/* 1 when the current panel has a last value. */
typedef struct Panel088CBB10 {
    char data[0xC5];
    unsigned char last;
} Panel088CBB10;

typedef struct Owner088CBB10 {
    char field_00;
    unsigned char fallback;
    char pad_02[0xE];
    Panel088CBB10 panels[9];
    char pad_706[0x773 - 0x10 - 9 * 0xC6];
    signed char current;
} Owner088CBB10;

int func_088CBB10(Owner088CBB10 *o)
{
    Panel088CBB10 *panels = o->panels;
    return panels[o->current].last > 0;
}
