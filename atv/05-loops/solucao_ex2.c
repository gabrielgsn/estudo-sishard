/* Implemente aqui sua versão em C da função soma_n
 * presente no arquivo ex2.o
 *
 * Chame sua função de soma_n_solucao */

//  0x0000000000000000 <+0>:     endbr64 
//  0x0000000000000004 <+4>:     mov    $0x0,%eax
//  0x0000000000000009 <+9>:     mov    $0x0,%edx
//  0x000000000000000e <+14>:    cmp    %edi,%eax
//  0x0000000000000010 <+16>:    jge    0x1d <soma_n+29>
//  0x0000000000000012 <+18>:    movslq %eax,%rcx
//  0x0000000000000015 <+21>:    add    %rcx,%rdx
//  0x0000000000000018 <+24>:    add    $0x1,%eax
//  0x000000000000001b <+27>:    jmp    0xe <soma_n+14>
//  0x000000000000001d <+29>:    mov    %rdx,%rax
//  0x0000000000000020 <+32>:    ret    

long soma_n_solucao(int edi){
    int eax = 0;
    long rdx = 0;
    while(eax < edi){
        rdx += eax;
        eax++;
    }
    return rdx;
}