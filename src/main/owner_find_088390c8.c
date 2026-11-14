typedef struct Agent4Member390C8 {
    unsigned char unknown_0000[0x1C];
    void *owner;
} Agent4Member390C8;

Agent4Member390C8 *func_0880844C(Agent4Member390C8 *previous);

int func_088390C8(void *owner) {
    Agent4Member390C8 *member = 0;
    while ((member = func_0880844C(member)) != 0) {
        if (member->owner == owner) {
            return 1;
        }
    }
    return 0;
}
