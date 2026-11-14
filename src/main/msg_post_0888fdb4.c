/* Posts a message with the current time stamp to the message queue. */
extern char D_09B491F8[];

int func_088908E0(void);
void func_0887AC70(void *queue, int a, int b, int time);

void func_0888FDB4(int a, int b)
{
    func_0887AC70(D_09B491F8, a, b, func_088908E0());
}
