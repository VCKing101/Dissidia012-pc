typedef struct Bind { unsigned short index; short field_02; unsigned short key; short field_06; } Bind;
extern void func_0887B924(void *task);

void func_088D45E4(void *owner, void **tasks, Bind *b)
{
    Bind *cursor = b;
    for (;;) {
        if (cursor->key == 0xFFFF) {
            break;
        }
        if (tasks[cursor->index] != 0) {
            func_0887B924(tasks[cursor->index]);
        }
        cursor++;
    }
}
