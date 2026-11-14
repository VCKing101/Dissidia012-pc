/* Stops the current voice, starts voice `index` (id from the table at
   D_08AB06C8) and refreshes the subtitle layout.
   (Not a stack-spill function.) */
typedef struct Data08AAA118 {
    char unknown_000[0x10];
    char layout[0x480];
    char screen[0x80];
    void *voice;
    char unknown_514[0x138];
    void *player;
    void *bank;
} Data08AAA118;

typedef struct Menu08AAA118 {
    char unknown_0000[0x7CFC];
    Data08AAA118 *data;
} Menu08AAA118;

extern unsigned short D_08AB06C8[];
extern char D_08AB0620[];
void func_0887BA5C(void *voice);
void *func_0887B3EC(void *player, void *bank, int a, unsigned short id, int b, int c);
void func_088D428C(void *layout, void *screen, void *name, int mode);

void func_08AAA118(Menu08AAA118 *self, int index)
{
    void *player = self->data->player;
    if (self->data->voice != 0) {
        func_0887BA5C(self->data->voice);
        self->data->voice = 0;
    }
    self->data->voice = func_0887B3EC(player, self->data->bank, 0, D_08AB06C8[index], 0, 0);
    func_088D428C(self->data->layout, self->data->screen, D_08AB0620, 1);
}
