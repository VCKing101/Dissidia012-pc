/* Looks up the message for a text id and shows it. */
typedef struct Game088B1354 {
    char pad_00[0xEE0];
    int messages;
} Game088B1354;

extern Game088B1354 *D_089D712C;
extern int D_089D7270[];

char *func_0887AFBC(int messages, int id, int flags);
void func_088B1230(int window, char *text, int a, int b, int c, int d);

void func_088B1354(int window, unsigned int index, int a, int b, int c)
{
    char *text = func_0887AFBC(D_089D712C->messages, D_089D7270[index], 0);
    func_088B1230(window, text, a, b, c, 0);
}
