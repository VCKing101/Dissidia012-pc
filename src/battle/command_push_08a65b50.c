/* Command queue pushes: take a free command and fill in its kind and
 * value (kinds 1-3) or rate (kind 4) (-1 when the queue is closed, -2 when it is full). */
typedef struct QueueCommand {
    int kind;
    union {
        int value;
        float rate;
    } arg;
} QueueCommand;

typedef struct CommandQueue {
    unsigned char open;
} CommandQueue;

extern QueueCommand *func_08A65CC0(CommandQueue *queue);

int func_08A65B50(CommandQueue *queue, int value)
{
    QueueCommand *command;
    if (!queue->open) {
        return -1;
    }
    command = func_08A65CC0(queue);
    if (command == 0) {
        return -2;
    }
    command->kind = 1;
    command->arg.value = value;
    return 0;
}

int func_08A65BAC(CommandQueue *queue, int value)
{
    QueueCommand *command;
    if (!queue->open) {
        return -1;
    }
    command = func_08A65CC0(queue);
    if (command == 0) {
        return -2;
    }
    command->kind = 2;
    command->arg.value = value;
    return 0;
}

int func_08A65C08(CommandQueue *queue, int value)
{
    QueueCommand *command;
    if (!queue->open) {
        return -1;
    }
    command = func_08A65CC0(queue);
    if (command == 0) {
        return -2;
    }
    command->kind = 3;
    command->arg.value = value;
    return 0;
}

int func_08A65C64(CommandQueue *queue, float rate)
{
    QueueCommand *command;
    if (!queue->open) {
        return -1;
    }
    command = func_08A65CC0(queue);
    if (command == 0) {
        return -2;
    }
    command->kind = 4;
    command->arg.rate = rate;
    return 0;
}
