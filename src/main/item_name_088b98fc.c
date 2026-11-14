typedef struct Item { char data_00[0x5E]; char name[2]; } Item;
typedef struct Info { short field_00; short text; } Info;
extern char D_09B4E828[];
extern int func_088C7F78(int id);
extern Item *func_088C7FE4(int id);
extern Info *func_088C0F8C(int id);
extern char *func_0887AFBC(void *table, int index, int flags);

char *func_088B98FC(int id)
{
    char *name;
    if (id == -1) {
        return 0;
    }
    name = 0;
    if (func_088C7F78(id) == 1) {
        Item *item = func_088C7FE4(id);
        if (item != 0) {
            name = item->name;
        }
    } else {
        Info *info = func_088C0F8C(id);
        name = func_0887AFBC(D_09B4E828, info->text, 0);
    }
    return name;
}
