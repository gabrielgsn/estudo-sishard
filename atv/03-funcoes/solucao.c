// TODO implemente aqui uma funcao chamada "solucao"

// 0x000000000000000a <+0>:     endbr64 
// 0x000000000000000e <+4>:     lea    (%rsi,%rsi,4),%eax
// 0x0000000000000011 <+7>:     add    %edi,%eax
// 0x0000000000000013 <+9>:     mov    %eax,(%rdx)
// 0x0000000000000015 <+11>:    lea    (%rsi,%rsi,2),%eax
// 0x0000000000000018 <+14>:    lea    (%rax,%rdi,4),%eax
// 0x000000000000001b <+17>:    ret
// rdi -> a
// rsi -> b
// rdx -> p

int solucao(int a, int b, int *p){
    int c = b + b*4;
    c += a;
    *p = c;
    c = b + b*2;
    c += a*4;
    return c;

}