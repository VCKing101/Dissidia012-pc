/* Count byte of the loaded size table; 0 when none is loaded. */
typedef struct SizeInfo088C6A7C {
    char pad_00[3];
    unsigned char count;
    unsigned char values[1];
} SizeInfo088C6A7C;

extern SizeInfo088C6A7C *D_089D7728;
unsigned char func_088C6A7C(void)
{
    if (D_089D7728 != 0) {
        return D_089D7728->count;
    }
    return 0;
}
