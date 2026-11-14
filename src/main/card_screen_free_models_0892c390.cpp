extern "C" {
typedef struct CardScreen { char pad_00[0x2C]; void *items[4]; void *models[2]; } CardScreen;
extern void func_088DA060(void *p);
void func_0892C390(CardScreen *s)
{
    for (int i = 0; i < 2; i++) {
        if (s->models[i] != 0) {
            func_088DA060(s->models[i]);
            s->models[i] = 0;
        }
    }
}
}
