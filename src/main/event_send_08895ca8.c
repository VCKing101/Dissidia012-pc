typedef struct Event08895CA8 {
    int value;
    int kind;
} Event08895CA8;

void func_08895CF8(void *self, Event08895CA8 *event);

void func_08895CA8(void *self, int value) {
    Event08895CA8 event;
    event.value = value;
    event.kind = 2;
    func_08895CF8(self, &event);
}
