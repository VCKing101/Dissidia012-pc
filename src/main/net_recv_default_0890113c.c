typedef struct SendOption {
    signed char value;
} SendOption;

extern int func_08900728(void *session, void *data, SendOption option);

int func_0890113C(void *session, void *data)
{
    SendOption option;

    option.value = 0;
    return func_08900728(session, data, option);
}
