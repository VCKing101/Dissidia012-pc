typedef struct Pager4E960 {
    int pages;
    char pad_04[0xC];
    unsigned char odd;
} Pager4E960;

extern Pager4E960 D_09B6DCE8;

void func_08A4E960(int self, int count)
{
    if (count == 0) {
        D_09B6DCE8.pages = 0;
    } else {
        count = count - 1;
        D_09B6DCE8.pages = count / 2 + 1;
        D_09B6DCE8.odd = (count % 2) == 0;
    }
}
