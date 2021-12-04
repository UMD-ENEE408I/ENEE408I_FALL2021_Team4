#ifndef TEMPLATE_FUNCTIONS
#define TEMPLATE_FUNCTIONS


template<typename T>        //prints out all the binary bits of a variable
void printBinary(T value){
    int num_bits = sizeof(value)*8;
    for(int i=0; i<num_bits; i++){
        Serial.print((value & (1<<num_bits-i-1)) ? "1" : "0");
    } Serial.println();
}

template<typename T>        //specify N number of bits to print out
void printBinaryN(T value, int N){
    int num_bits = sizeof(value)*8;
    if(N > num_bits) Serial.println("\nError using printBinaryN(). Specified N value it too large.\n\n");
    
    for(int i=0; i<N; i++){
        Serial.print((value & (1<<N-i-1)) ? "1" : "0");
    } Serial.println();
}


#endif
