// TODO: sua função deverá se chamar "solucao"

void solucao(short rdi[], short rsi[], int edx){
    edx--;;
    short eax;
    while(edx >= 0){
        eax = rdi[edx];
        if(eax <= 0){
            eax = -1*eax;
        }
        rsi[edx] = eax;
        edx--;
    }
}
