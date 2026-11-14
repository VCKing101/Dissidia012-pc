/* Starts loading the save block (alternate mode unless func_08AF96B0 reports set). */
typedef struct LoadState {
    char pad_0000[0x2BFC];
    int field_2BFC;
    char pad_2C00[0x2CCC - 0x2C00];
    char field_2CCC[0x10];
} LoadState;

typedef struct LoadOwner {
    char pad_000[0x140];
    LoadState *field_140;
} LoadOwner;

extern char D_08B00296[];
extern int func_08AF96B0(void);
extern void func_088D5A00(void *file, void *data, void *block, void *name, unsigned char fresh, int one,
                          int size);

void func_08AF3930(LoadOwner *self, void *file, void *data)
{
    void *block = self->field_140->field_2CCC;
    char *name = D_08B00296;
    int alternate = func_08AF96B0();
    func_088D5A00(file, data, block, name, alternate == 0, 1, self->field_140->field_2BFC);
}
