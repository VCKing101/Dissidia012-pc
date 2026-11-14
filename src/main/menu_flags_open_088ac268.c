/* Sets flag bits 14 and 15, then opens the menu page. */
typedef struct Menu088AC268 {
    char pad_00[0xEF8];
    union {
        unsigned int word;
        struct {
            unsigned int low : 14;
            unsigned int b14 : 1;
            unsigned int b15 : 1;
        } bits;
    } flags;
} Menu088AC268;

void func_088AC82C(Menu088AC268 *menu, int open);

void func_088AC268(Menu088AC268 *menu)
{
    menu->flags.bits.b14 = 1;
    menu->flags.bits.b15 = 1;
    func_088AC82C(menu, 1);
}
