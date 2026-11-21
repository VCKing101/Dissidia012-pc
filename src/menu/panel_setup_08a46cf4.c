/* Runs the steps of the panel: layout, cells, slot marks, title, texts. */
typedef struct Work46CF4 {
    char pad_000[0x3F4];
    int field_3F4;            /* +0x3F4 */
} Work46CF4;

typedef struct Screen46CF4 {
    char pad_000[0x418];
    Work46CF4 *work;          /* +0x418 */
} Screen46CF4;

extern unsigned short D_08A4810A;
void func_08A4668C(Screen46CF4 *self, int b, int c, int d);
void func_08A46538(Screen46CF4 *self, int a);
void func_08A46ABC(Screen46CF4 *self);
void func_08A4639C(Screen46CF4 *self, unsigned short id);
void func_08A46768(Screen46CF4 *self, int one, int a, int b, int d, int a2, int b2, int d2);
void func_08A468E0(Screen46CF4 *self, int a, int b, int d);

void func_08A46CF4(Screen46CF4 *self, int a, int b, int c)
{
    int d;

    func_08A4668C(self, b, c, 0);
    d = self->work->field_3F4;
    func_08A46538(self, a);
    func_08A46ABC(self);
    func_08A4639C(self, D_08A4810A);
    func_08A46768(self, 1, a, b, d, a, b, d);
    func_08A468E0(self, a, b, d);
}
