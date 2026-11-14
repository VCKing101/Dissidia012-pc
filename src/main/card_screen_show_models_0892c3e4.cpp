extern "C" {
typedef struct CardScreen { char pad_00[0x2C]; void *items[4]; void *models[2]; } CardScreen;
extern void func_088D9B28(void *p, unsigned char on);
void func_0892C3E4(CardScreen *s, unsigned char on)
{
    for (int i = 0; i < 2; i++) {
        if (s->models[i] == 0) {
            continue;
        }
        func_088D9B28(s->models[i], on);
    }
}
}
