typedef struct Event08895C58 {
    int value;
    int kind;
} Event08895C58;

void func_08895CF8(void *self, Event08895C58 *event);

void func_08895C58(void *self, int value) {
    Event08895C58 event;
    event.value = value;
    event.kind = 1;
    func_08895CF8(self, &event);
}
