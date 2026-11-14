/* Command queue pushes of the block markers: kind 0 with value 0 (block
 * start) and with value 1 (block end). */
typedef struct QueueCommand {
    int kind;
    int value;
} QueueCommand;

typedef struct CommandQueue {
    unsigned char open;
} CommandQueue;

extern QueueCommand *func_08A65CC0(CommandQueue *queue);

int func_08A65AAC(CommandQueue *queue)
{
    QueueCommand *command;
    if (!queue->open) {
        return -1;
    }
    command = func_08A65CC0(queue);
    if (command == 0) {
        return -2;
    }
    command->kind = 0;
    command->value = 0;
    return 0;
}

int func_08A65AFC(CommandQueue *queue)
{
    QueueCommand *command;
    if (!queue->open) {
        return -1;
    }
    command = func_08A65CC0(queue);
    if (command == 0) {
        return -2;
    }
    command->kind = 0;
    command->value = 1;
    return 0;
}
