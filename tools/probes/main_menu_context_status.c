typedef struct MainMenuContextPrefix {
    unsigned char unknown_0000[0xEF8];
    unsigned int field_0EF8;
} MainMenuContextPrefix;

int func_088AC890(void *object) {
    unsigned int flags = ((MainMenuContextPrefix *)object)->field_0EF8;
    int result;
    if (flags & 4) result = 0;
    else if (flags & 0x800) result = 1;
    else result = -1;
    return result;
}
