#include "stdbool.h"

int day_of_the_week(int year, int month, int day) {
    int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    if (month < 3) {
        year -= 1;
    }
    return (year + year / 4 - year / 100 + year / 400 - year / 4000 + t[month - 1] + day) % 7;
}

int isLeapYear(int year){
    if(year % 4 == 0 ){
        if(year % 100 == 0){
            if(year % 400 == 0){
                if(year % 4000 == 0){
                    return 0;
                }
                else return 1;
            }
        }
        else return 1;
    } else {
        return 0;
    }
    return 0;
}

bool isHoliday(int day, int month, int year){
    int whichDay = day_of_the_week(year, month, day);
    if(whichDay == 7 || whichDay == 6 || whichDay == 0){
        return true;
    }
    if((month == 1 && day == 1) || (month == 5 && day == 1) || (month == 5 && day == 8) || (month == 7 && day == 5)
       || (month == 7 && day == 6) || (month == 9 && day == 28) || (month == 10 && day == 28) || (month == 11 && day == 17)
       || (month == 12 && day == 24) || (month == 12 && day == 25) || (month == 12 && day == 26)){
        return true;
    }
    return false;
}

bool isWorkDay(int y, int m, int d){
    int month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(isLeapYear(y) == 1){
        month_days[2] = 29;
    } else {
        month_days[2] = 28;
    }
    if(y < 2000 || m < 1 || m > 12 || d > month_days[m] || d < 1){
        return false;
    }
    if(isHoliday(d, m, y) == true){
        return false;
    }
    return true;
}

TResult countDays(int y1, int m1, int d1, int y2, int m2, int d2) {
    int month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    TResult invalidInput = {-1,-1}, result = {0,0};
    if(isLeapYear(y1) == 1){
        month_days[2] = 29;
    }
    if(d1 > month_days[m1]){
        return invalidInput;
    }
    month_days[2] = 28;
    if(isLeapYear(y2) == 1){
        month_days[2] = 29;
    }
    if(d2 > month_days[m2]){
        return invalidInput;
    }
    month_days[2] = 28;
    if(y1 < 2000 || y2 < 2000 || m1 > 12 || m1 < 1 || m2 > 12 || m2 < 1 || d1 < 1
       || d2 < 1 || y1 > y2 || (y1 == y2 && m1 == m2 && d1 > d2) || (y1 == y2 && m1 > m2)){
        return invalidInput;
    }

    if(y1 == y2){
        int workDaySame = 0, totalDaySame = 0;
        if(isLeapYear(y1) == 1){
            month_days[2] = 29;
        } else {
            month_days[2] = 28;
        }
        int daySame = d1;
        for(int monthSame = m1; monthSame <= m2; monthSame++){
            int dayLim;
            if(monthSame == m2){
                dayLim = d2;
            } else {
                dayLim = month_days[monthSame];
            }
            for(; daySame <= dayLim; daySame++){
                totalDaySame++;
                if(isHoliday(daySame, monthSame, y1) == false){
                    workDaySame++;
                }
            }
            daySame = 1;
        }
        result.m_TotalDays = totalDaySame;
        result.m_WorkDays = workDaySame;
        return result;
    }

    if(isLeapYear(y1) == 1){
        month_days[2] = 29;
    } else {
        month_days[2] = 28;
    }
    int dayBegin = d1, monthBegin = m1, workDays1 = 0, totalDays1 = 0;
    for(; monthBegin <= 12; monthBegin++){
        for(; dayBegin <= month_days[monthBegin]; dayBegin++){
            totalDays1++;
            if(isHoliday(dayBegin, monthBegin, y1) == false){
                workDays1++;
            }
        }
        dayBegin = 1;
    }

    if(isLeapYear(y2) == 1){
        month_days[2] = 29;
    } else {
        month_days[2] = 28;
    }
    int dayEnd = d2, monthEnd = m2, workdays2 = 0, totalDays2 = 0;
    for(int month = 1; month <= monthEnd; month++){
        int dayLim;
        if(month == monthEnd){
            dayLim = dayEnd;
        } else {
            dayLim = month_days[month];
        }
        for(int day = 1; day <= dayLim; day++){
            totalDays2++;
            if(isHoliday(day, month, y2) == false){
                workdays2++;
            }
        }
    }

    int yearInterval = y1 + 1, yearInterval2 = y2 - 1, workworkdays = 0, totaltotaldays = 0;
    for(; yearInterval <= yearInterval2; yearInterval++){
        if(isLeapYear(yearInterval) == 1){
            int whichdayforLeap = day_of_the_week(yearInterval, 1, 1);
            if(whichdayforLeap == 1 || whichdayforLeap == 5 || whichdayforLeap == 2){
                workworkdays += 254;
                totaltotaldays += 366;
            } else if(whichdayforLeap == 3 || whichdayforLeap == 0){
                workworkdays += 253;
                totaltotaldays += 366;
            } else if(whichdayforLeap == 4){
                workworkdays += 255;
                totaltotaldays += 366;
            } else if(whichdayforLeap == 6){
                workworkdays += 252;
                totaltotaldays += 366;
            }
        } else {
            int whichdayforLeap = day_of_the_week(yearInterval, 1, 1);
            if(whichdayforLeap == 1 || whichdayforLeap == 4 || whichdayforLeap == 0){
                workworkdays += 252;
                totaltotaldays += 365;
            } else if(whichdayforLeap == 6 || whichdayforLeap == 5){
                workworkdays += 254;
                totaltotaldays += 365;
            } else if(whichdayforLeap == 2 || whichdayforLeap == 3){
                workworkdays += 253;
                totaltotaldays += 365;
            }
        }
    }

    result.m_WorkDays = workworkdays + workDays1 + workdays2;
    result.m_TotalDays = totaltotaldays + totalDays1 + totalDays2;

    return result;
}
