/* Implemente aqui sua versão em C da função ex3
 * presente no arquivo ex3.o
 * 
 * Chame sua função de ex3_solucao */

//  0x0000000000000000 <+0>:     endbr64 
//  0x0000000000000004 <+4>:     mov    $0x0,%ecx
//  0x0000000000000009 <+9>:     mov    $0x0,%r8d
//  0x000000000000000f <+15>:    jmp    0x15 <ex3+21>
//  0x0000000000000011 <+17>:    add    $0x1,%rcx
//  0x0000000000000015 <+21>:    cmp    %rdi,%rcx
//  0x0000000000000018 <+24>:    jge    0x2c <ex3+44>
//  0x000000000000001a <+26>:    mov    %rcx,%rax
//  0x000000000000001d <+29>:    cqto   
//  0x000000000000001f <+31>:    idiv   %rsi
//  0x0000000000000022 <+34>:    test   %rdx,%rdx
//  0x0000000000000025 <+37>:    jne    0x11 <ex3+17>
//  0x0000000000000027 <+39>:    add    %rcx,%r8
//  0x000000000000002a <+42>:    jmp    0x11 <ex3+17>
//  0x000000000000002c <+44>:    mov    %r8,%rax
//  0x000000000000002f <+47>:    ret

long ex3_solucao(int rdi, int rsi){
    int rcx = 0;
    long r8 = 0;
    int rdx = 0;
    while(rcx < rdi){
        if(rcx%rsi == 0){
            r8 += rcx;
        }
        rcx++;
    }
    return r8;
}