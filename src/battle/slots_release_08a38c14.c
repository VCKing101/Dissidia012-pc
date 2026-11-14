typedef struct HandleSlot {
    void *handle;
    int field_04;
} HandleSlot;

typedef struct SlotData {
    int state;
    int next;
    char pad_08[0x8];
    HandleSlot slots[6];
    char pad_40[0x14];
    void *model;
    void *body;
} SlotData;

typedef struct SlotOwner {
    char pad_00[0x14];
    SlotData *data;
} SlotOwner;

extern void func_0887BA5C(void *handle);
extern void func_088D9CFC(void *model);
extern void func_088D9F5C(void *model, int motion, int restart);

/* Releases the six slot handles, stops the model and puts the body in motion 4 (state 2). */
void func_08A38C14(SlotOwner *self)
{
    int i;
    for (i = 0; i < 6; i++) {
        if (self->data->slots[i].handle != 0) {
            func_0887BA5C(self->data->slots[i].handle);
            self->data->slots[i].handle = 0;
        }
    }
    func_088D9CFC(self->data->model);
    func_088D9F5C(self->data->body, 4, 1);
    self->data->state = 2;
    self->data->next = 2;
}
