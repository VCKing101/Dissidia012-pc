#include "menu/agent29_types.h"

/* Screen0B6A0: plays the card page script and makes its texts: the card's
   name (texts[0x16]), the count owned (texts[0x1B], from the save record)
   and the caption (texts[0x17]). */
extern "C" {
extern const unsigned short D_08A09CB2[];
extern const unsigned short D_08A09CDA[];
extern const char D_08A0AC54[];
int func_088D6254(int card);
void func_0887DF14(void *owner, char *out, int size, const char *text);
}

/* Inlined call on the object in D_089D6998. */
struct Object9D6998 {
    static void Convert(char *out, int size, const char *text) {
        func_0887DF14(D_089D6998, out, size, text);
    }
};

extern "C" void func_089F46E8(Screen0B6A0 *self)
{
    char text[0x40];
    char *args[1];
    int card;
    unsigned short name;
    Record8CB164 *record;

    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09CB2);
    card = WORK_SHORT(self->work, 0x3166);
    name = func_088D6254(card);
    if (name != 0xFFFF) {
        self->work->texts[0x16] = func_0887B3EC(self->work->messages, 1, 0, name, 0, 0);
    }
    record = func_088CB164(D_089D712C->field_F58, card);
    func_0893476C(text, (int)D_08A0AC54, record->field_01);
    Object9D6998::Convert((char *)self->work + 0x2078, 0x80, text);
    args[0] = (char *)self->work + 0x2078;
    self->work->texts[0x1B] = func_089EF1E8(self, 0, 9, (int)args, 1);
    self->work->texts[0x17] = func_089EF1E8(self, 0, 0x77, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_08A09CDA, 1);
    func_089F3F68(self, 0xA5);
    func_089F3F68(self, 0xAA);
}
