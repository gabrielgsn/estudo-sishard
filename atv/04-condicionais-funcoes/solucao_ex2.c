long vezes2(long a);

/* Implementar função solucao_ex2 */
// 0x0000000000001132 <+0>:     endbr64 
// 0x0000000000001136 <+4>:     push   %rbx
// 0x0000000000001137 <+5>:     mov    %rdi,%rbx
// 0x000000000000113a <+8>:     mov    %rsi,%rdi
// 0x000000000000113d <+11>:    call   0x1129 <vezes2>
// 0x0000000000001142 <+16>:    cmp    %rbx,%rax
// 0x0000000000001145 <+19>:    jle    0x114a <ex2+24>
// 0x0000000000001147 <+21>:    add    %rbx,%rbx
// 0x000000000000114a <+24>:    add    %rbx,%rax
// 0x000000000000114d <+27>:    pop    %rbx
// 0x000000000000114e <+28>:    ret  

long solucao_ex2(long rdi, long rsi){
    long rbx;
    rbx = rdi;
    rdi = rsi;
    if(vezes2(rdi) <= rbx){
        return rbx + vezes2(rdi);
    }
    return rbx + rbx + vezes2(rdi);
}