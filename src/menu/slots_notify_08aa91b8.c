/* Notifies each of the five slots at +0x514 of the menu data, through
   func_0887B944 or func_0887B924 depending on the flag at +0x7CB8.
   (Not a stack-spill function: the callees take the slot, which is why it
   sits in a0.) */
typedef struct Data08AA91B8 {
    char unknown_0000[0x514];
    void *slots[5];
    char unknown_0528[0x7790];
    int alternate;
} Data08AA91B8;

typedef struct Menu08AA91B8 {
    char unknown_0000[0x7CFC];
    Data08AA91B8 *data;
} Menu08AA91B8;

void func_0887B944(void *slot);
void func_0887B924(void *slot);

void func_08AA91B8(Menu08AA91B8 *self)
{
    int i;
    for (i = 0; i < 5; i++) {
        void *slot = self->data->slots[i];
        if (slot != 0) {
            if (self->data->alternate == 0) {
                func_0887B944(slot);
            } else {
                func_0887B924(slot);
            }
        }
    }
}
