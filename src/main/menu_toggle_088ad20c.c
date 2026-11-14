/* Advances the menu step, closes an open page, or marks the page as opening. */
typedef struct Menu088AD20C {
    char pad_00[0xEE4];
    int handle;
    int steps;
    char pad_EEC[0xEF8 - 0xEEC];
    unsigned int flags;
    char pad_EFC[0xF04 - 0xEFC];
    int page;
} Menu088AD20C;

void func_088AD2B0(Menu088AD20C *menu);
void func_0899BB1C(int handle);
void func_088822B4(int item);

void func_088AD20C(Menu088AD20C *menu, int item)
{
    unsigned int flags = menu->flags;
    if (flags & 0x10100) {
        func_088AD2B0(menu);
        menu->steps++;
    } else if (flags & 0x1000) {
        if (menu->handle != 0) {
            func_0899BB1C(menu->handle);
            menu->handle = 0;
        }
        func_088822B4(item);
        menu->page = 0;
        menu->flags &= ~0x1000;
    } else {
        menu->flags = flags | 0x1000;
    }
}
