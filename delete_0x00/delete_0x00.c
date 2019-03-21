#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void data_cleaning(char* data, int length ){

    char* abc = data;
    int total_length = length;
    int i = 0, j = 0;
    int temp_length = 0;

    for(i=0; i<total_length; i+=temp_length ){
        temp_length = strlen(abc);
        if((i+temp_length)>total_length)
            temp_length = temp_length - ((strlen(abc)+i) % total_length);
        printf("%d: ", temp_length);

        for(j=0; j<temp_length; j++){
            printf("%2x ", (unsigned char)abc[j]);
        }
        printf("\n");

        temp_length += 1;
        abc += temp_length;

    }
}

int main(){

    int m = 0;
    char a[] = {0x00,0xAC,0x33,0x64,0x14,0xAB,0x00,0x80,0x64,0x14,0xAB,0x00,0x5C,0x64,0x14,0xAB,0x00,0x5C,0x00,0x55};
    printf("a has %d nums: ", sizeof(a));
    for(m=0; m<sizeof(a); m++) {
        printf("%2x ", (unsigned char)a[m]);
    }
    printf("\n");

    data_cleaning( a, sizeof(a) );

    return 0;
}
