typedef struct SendOption {
    signed char value;
} SendOption;

extern int func_089007C0(void *session, void *data, SendOption option);

int func_0890115C(void *session, void *data)
{
    SendOption option;

    option.value = 0;
    return func_089007C0(session, data, option);
}
