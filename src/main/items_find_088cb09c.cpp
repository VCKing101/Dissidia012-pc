extern "C" {
extern void *D_09B69868[4];
int func_088CAFAC(void *item, void *arg);

int func_088CB09C(void *arg)
{
    int found = 0;
    for (int i = 0; i < 4; i++) {
        if (D_09B69868[i] != 0 && (found = func_088CAFAC(D_09B69868[i], arg)) != 0) {
            break;
        }
    }
    return found;
}
}
