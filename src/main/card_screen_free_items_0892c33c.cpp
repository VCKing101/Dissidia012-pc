extern "C" {
typedef struct CardScreen { char pad_00[0x2C]; void *items[4]; void *models[2]; } CardScreen;
extern void func_088DAB44(void *p);
void func_0892C33C(CardScreen *s)
{
    for (int i = 0; i < 4; i++) {
        if (s->items[i] != 0) {
            func_088DAB44(s->items[i]);
            s->items[i] = 0;
        }
    }
}
}
