/* Draws the frame and its cursor layer unless func_08B0DF10 says not to. */
struct Layout08B05254 {
    char pad_000[4];
    char field_004[0x400];
    char field_404[0x884 - 0x404];
    char field_884[4];
};

struct Owner08B05254 {
    char pad_000[0x138];
    struct Layout08B05254 *field_138;
};

extern unsigned short D_08B1BD04[];
extern unsigned short D_08B1BD14[];
extern int func_08B0DF10(struct Owner08B05254 *self);
extern void func_088D3EA0(char *owner, char *layout, unsigned short *table);
extern void func_088D4664(char *layout, char *cursor, unsigned short *table);

void func_08B05254(struct Owner08B05254 *self)
{
    if (func_08B0DF10(self) != 0) {
        return;
    }
    func_088D3EA0(self->field_138->field_004, self->field_138->field_404, D_08B1BD04);
    func_088D4664(self->field_138->field_404, self->field_138->field_884, D_08B1BD14);
}
