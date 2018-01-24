/*************************************************

Copyright:free

Author:Isaac

Date:2017-09-20

Description:A calendar from january 1,1900,Monday

**************************************************/

#include<stdio.h>

//#define PRINT_IT    1

#define LEAP_YEAR 366
#define COMMON_YEAR 365

int judge_leap_year(int);
int count_leap_years(int);
int get_month_days(int, int);
long get_total_days(int, int);
void print_calendar(int , int);


enum Weekdays{
    Sunday    = 0,
    Monday    = 1,
    Tuesday   = 2,
    Wednesday = 3,
    Thursday  = 4,
    Friday    = 5,
    Saturday  = 6
};

enum Monday{
    January   = 1,
    February  = 2,
    March     = 3,
    April     = 4,
    May       = 5,
    June      = 6,
    July      = 7,
    August    = 8,
    September = 9,
    October   = 10,
    November  = 11,
    December  = 12,
};

int main(void){

	int year = 0, month = 0;
	int i = 0;

    printf("input the year and month that you want to print\n");

    scanf("%d %d", &year, &month);
    while( (year<1900) || (month>12) || (month<0) ){
        printf("input the year and month that you want to print again\n");
        scanf("%d %d", &year, &month);
    }

    //print_calendar(year, month);

    for(i = 1; i<13 ; i++){
        print_calendar(year, i);
    }


    return 1;
}

/**********************************************************************/
/* judge this year is a leap year or a common year,
 * return 1 if it's a leap year.
 * Parameters: the year
 * Returns: 1 if the year is a leap year, else 0 */
/**********************************************************************/
int judge_leap_year(int year){

    if( ((year % 4 == 0)&&(year % 100 != 0)) || (year % 400 == 0) ) return 1;
    else return 0;
}

/**********************************************************************/
/* count the leap year from 1900 to the year input
 * Parameters: the year
 * Returns: the leap years */
/**********************************************************************/
int count_leap_years(int year){

    int i = 0, leapyearnum = 0;
#ifdef PRINT_IT
        printf("the leap year from 1900 as below:\n");
#endif
    for( i=1900; i <= year; i++ )
        if( judge_leap_year(i) ){
                leapyearnum++;
#ifdef PRINT_IT
                printf("%d\n", i);
#endif
        }
#ifdef PRINT_IT
        printf("the number of leap years is:");
        printf("%d\n", leapyearnum);
#endif
    return leapyearnum;
}


/**********************************************************************/
/* ensure the days of the month
 * Parameters: the month, the year
 * Returns: the days of the month */
/**********************************************************************/
int get_month_days(int year,int month){

    int month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};

    if(month!=2)return month_days[month-1];
    else if( judge_leap_year(year) )return 29;
    else return 28;
}


//get the total day from 1900.01.01 to the last day of last month
long get_total_days(int year,int month){

    long days = 0;
    int common[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int leap[12] = {0,31,60,91,121,152,182,213,243,274,305,335};
    int leap_years = 0;

    leap_years = count_leap_years( year - 1) ;

    days = leap_years * LEAP_YEAR + (year - 1900 - leap_years) * COMMON_YEAR;

    if(judge_leap_year(year))
        days += leap[month-1];
    else
        days += common[month-1];

    return days;
}

void print_calendar(int year, int month){

    int firstday = 0; //record the first day of the month
    int i = 0, j = 0, n = 1;
    int month_days = 0;
    long total_days = 0;

    count_leap_years(year);
    total_days = get_total_days(year, month);
    firstday = total_days % 7 + 1;
    month_days = get_month_days(year, month);

    printf("this month is:%d\n", month);
    printf("the total days before this month is:%d\n", total_days);
    printf("the firstady of this month is:%d\n", firstday);
    printf("the days of this month is:%d\n", month_days);

    printf("______________________\n\n");
    printf("  7  1  2  3  4  5  6 \n");

    if(firstday != 7){
        for(i = firstday; i > 0; i--)
            printf("   ");
    }

    for(j = month_days; j>0; j--){
        printf("%3d" , n);

        if( (n + firstday) % 7 == 0 )
            printf("\n");
        n++;
    }
    printf("\n_____________________\n\n");
}




//                                  _oo8oo_
//                                 o8888888o
//                                 88" . "88
//                                 (| -_- |)
//                                 0\  =  /0
//                               ___/'==='\___
//                             .' \\|     |// '.
//                            / \\|||  :  |||// \
//                           / _||||| -:- |||||_ \
//                          |   | \\\  -  /// |   |
//                          | \_|  ''\---/''  |_/ |
//                          \  .-\__  '-'  __/-.  /
//                        ___'. .'  /--.--\  '. .'___
//                     ."" '<  '.___\_<|>_/___.'  >' "".
//                    | | :  `- \`.:`\ _ /`:.`/ -`  : | |
//                    \  \ `-.   \_ __\ /__ _/   .-` /  /
//                =====`-.____`.___ \_____/ ___.`____.-`=====
//                 \\ \\ \\ \\ \\ \\`=---=` // // // // // //
//                   \\ \\ \\ \\ \\ \\ || // // // // // //
//                     \\ \\ \\ \\ \\ \\// // // // // //
//                      --------------------------------
//                       Buddha blesses me away from BUGs

/**********//**********//**********//**********//**********//**********//**********/




