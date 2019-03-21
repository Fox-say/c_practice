#include<stdio.h>
#include<string.h>

unsigned int letter=0,space=0,number=0,other=0;
char s[100];

void count_characters(char ,unsigned int );

void count_characters(char *s,unsigned int num){

    unsigned int i = 0;
    for(i=0;i<num;i++){
        if((s[i]>='a'&&s[i]<='z') || (s[i]>='A'&&s[i]<='Z')){
            letter++;
        }
        else if(s[i]>='0'&&s[i]<='9'){
            number++;
        }
        else if(s[i]==' '){
            space++;
        }
        else{
            other++;
        }
    }
}

void main(void){

	unsigned int n;

	printf("输入一行字符:\n");

	gets(s);
	n=strlen(s);
	count_characters(s,n);

	printf("%d个字母\n%d个数字\n%d个空格\n%d个其他字符",letter,number,space,other);

	while(1){

	}
}
