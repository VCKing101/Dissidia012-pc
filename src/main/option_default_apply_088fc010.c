/* Applies the default value of option `key` from the -1 terminated defaults table. */
typedef struct OptionDefault {
    int key;
    int index;
    int value;
} OptionDefault;

extern OptionDefault D_089D7F6C[];
extern void func_088F4870(int index, unsigned char value);

void func_088FC010(void *self, int key)
{
    OptionDefault *d = D_089D7F6C;

    for (;;) {
        if (d->key == -1) {
            break;
        }
        if (d->key == key) {
            func_088F4870(d->index, d->value);
            break;
        }
        d++;
    }
}
