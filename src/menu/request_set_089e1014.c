/* Posts a request with a value to the global menu request block. */
typedef struct MenuRequest {
    int state;
    int value;
    int result;
    int pending;
} MenuRequest;

extern MenuRequest D_09B6D830;

void func_089E1014(int unused, int value)
{
    MenuRequest *request = &D_09B6D830;
    request->value = value;
    D_09B6D830.state = 2;
    request->pending = 1;
}
