typedef struct Event08895CD0 {
    int value;
    int kind;
} Event08895CD0;

void func_08895D44(void *self, Event08895CD0 *event);

void func_08895CD0(void *self, int value) {
    Event08895CD0 event;
    event.value = value;
    event.kind = 2;
    func_08895D44(self, &event);
}
