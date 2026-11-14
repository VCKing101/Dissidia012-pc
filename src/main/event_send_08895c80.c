typedef struct Event08895C80 {
    int value;
    int kind;
} Event08895C80;

void func_08895D44(void *self, Event08895C80 *event);

void func_08895C80(void *self, int value) {
    Event08895C80 event;
    event.value = value;
    event.kind = 1;
    func_08895D44(self, &event);
}
