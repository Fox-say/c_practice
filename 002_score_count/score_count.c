#include <stdio.h>

unsigned char grade[20] = {};  //storage scores
unsigned char length = 0;      //score quantity
unsigned char range[2] = {};   //score range need to be count
unsigned char value = 0;
unsigned char range_grade[20] = {}; //scores in the range

int main(void){

    unsigned char temp = 0;
    unsigned char i = 0;
    unsigned char j = 0;

    printf("Please enter the number and range of scores that need statistics\n");
    printf("(Use space to separate and the greatest amount is 20)\n");
    printf("For example: \n3 50-70\n44 66 88\n");

    scanf("%d %d-%d", &length, &range[0], &range[1]);

    for(i=0 ; i<length ; i++){ 
        scanf("%d", &grade[i]);
    }

    for(i=0 ; i<length ; i++){  
        if( grade[i] >= range[0] && grade[i] <= range[1] ){
            range_grade[value++] = grade[i];
        }
    }
//sort
    for( i=0 ; i<value ; i++ ){
        for( j=0 ; j< value-1 ; j++){
            if(range_grade[j] > range_grade[j+1] ){
                temp = range_grade[j];
                range_grade[j] = range_grade[j+1];
                range_grade[j+1] = temp;
            }
        }
    }
    printf("score in the range %d-%d : %d\n", range[0], range[1], value);
    for(i=0 ; i< value ; i++){
        printf("%d ",range_grade[i]);
    }
    printf("\n");

    return 0;
}
