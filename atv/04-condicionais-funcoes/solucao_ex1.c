/* Implementar função solucao_ex1 */

// 0x0000000000000000 <+0>:     endbr64 
// 0x0000000000000004 <+4>:     lea    (%rdi,%rsi,1),%rax
// 0x0000000000000008 <+8>:     lea    (%rax,%rdx,4),%rcx
// 0x000000000000000c <+12>:    imul   %rdi,%rdi
// 0x0000000000000010 <+16>:    lea    (%rdi,%rsi,2),%rax
// 0x0000000000000014 <+20>:    add    %rax,%rdx
// 0x0000000000000017 <+23>:    cmp    %rdx,%rcx
// 0x000000000000001a <+26>:    setge  %al
// 0x000000000000001d <+29>:    movzbl %al,%eax
// 0x0000000000000020 <+32>:    ret  

int solucao_ex1(int rdi, int rsi, int rdx){
    int rax = rdi + rsi;
    int rcx = rax + 4*rdx;
    rdi = rdi * rdi;
    rax = rdi + 2*rsi;
    rdx = rax + rdx;
    if(rcx >= rdx){
        return 1;
    }
    return 0;
}

