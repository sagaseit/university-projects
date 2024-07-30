#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

typedef struct Day{
    int year, month, day;
    int startingIndex, endingIndex;
}Day;

typedef struct Period{
    int y1, y2, m1, m2, d1, d2;
    double average, difference;
    int median, startingIndex, endingIndex, review_count;
}Period;

typedef struct Review{
    int year, month, day;
    int rate;
    char message[4098];
}Review;

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

int dayInMonth(int year, int month){
    int month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(isLeapYear(year) == 1 && month == 2){
        return 29;
    } else if(isLeapYear(year) == 0 && month == 2){
        return 28;
    }
    return month_days[month];
}

int compare(const void *a, const void *b){
    return (*(int*)a - *(int*)b);
}

int medianInArray(int array[], int count){
    if (count % 2 == 0) {
        int mid1 = array[count / 2 - 1];
        int mid2 = array[count / 2];
        if(mid1 < mid2){
            return mid1;
        }
        return mid2;
    } else {
        return array[count / 2];
    }
}

void error(Review *reviews, Day *days, Period *periods){
    printf("Nespravny vstup.\n");
    free(reviews);
    free(days);
    free(periods);
}

int main() {
    int year, month, day, rate;
    char message[4098], sign;
    int capacity = 5, count = 0;
    int dayCapacity = 5, dayCount = 0;
    int perCapacity = 5, perCount = 0;
    Review *reviews = (Review *) malloc(sizeof(Review) * capacity);
    Day *days = (Day *) malloc(sizeof(Day) * dayCapacity);
    Period *periods = (Period *) malloc(sizeof(Period) * perCapacity);
    printf("Recenze:\n");
    while (1){
        int eof = scanf(" %c", &sign);
        if(eof == EOF){
            break;
        }
        if(sign == '+'){
            int tmp = scanf(" %d-%d-%d %d %4097s", &year, &month, &day, &rate, message);
            if (tmp != 5 || year < 1800 || year > 3000 || month > 12 || month < 1 || day < 1 || rate < 1 || day > dayInMonth(year, month)) {
                error(reviews, days, periods);
                return 1;
            }
            if(count >= 1) {
                if (reviews[count - 1].year > year || (reviews[count - 1].year == year && reviews[count - 1].month > month) ||
                    (reviews[count - 1].year == year && reviews[count - 1].month == month && reviews[count - 1].day > day)) {
                    error(reviews, days, periods);
                    return 1;
                }
            }
            if(count == capacity){
                capacity *= 2;
                Review *temporary = (Review*) realloc(reviews, capacity * sizeof(Review));
                if(temporary == NULL){
                    printf("Nespravny vstup.\n");
                    error(reviews, days, periods);
                }
                reviews = temporary;
            }
            reviews[count].year = year;
            reviews[count].month = month;
            reviews[count].day = day;
            reviews[count].rate = rate;
            strcpy(reviews[count].message, message);

            if(count == 0 || days[dayCount].day != day || days[dayCount].month != month || days[dayCount].year != year){
                if(count != 0) dayCount++;
                if(dayCapacity == dayCount){
                    dayCapacity *= 2;
                    Day *temporary = (Day*) realloc(days, dayCapacity * sizeof(Day));
                    days = temporary;
                }
                days[dayCount].day = day;
                days[dayCount].month = month;
                days[dayCount].year = year;
                days[dayCount].startingIndex = count;
                days[dayCount].endingIndex = count;
            } else {
                days[dayCount].endingIndex = count;
            }
            count++;
        } else if(sign == '?'){
            int min;
            if(scanf(" %d", &min) != 1 || min < 1){
                error(reviews, days, periods);
                return 1;
            }
            if(count == 0){
                error(reviews, days, periods);
                return 1;
            }
            if(min > count){
                printf("Neexistuje.\n");
                continue;
            }
            for(int i = 0; i <= dayCount; i++){
                for(int j = i; j <= dayCount; j++){
                    if(((days[j].endingIndex + 1) - days[i].startingIndex) < min){
                        continue;
                    }
                    int numbers[100000], numbersCount = 0;
                    double average;
                    int median;
                    if(perCapacity == perCount){
                        perCapacity *= 2;
                        Period *temporary = (Period*) realloc(periods, perCapacity * sizeof(Period));
                        periods = temporary;
                    }
                    double sum = 0;
                    for(int k = days[i].startingIndex; k <= days[j].endingIndex; k++){
                        sum += reviews[k].rate;
                        numbers[numbersCount] = reviews[k].rate;
                        numbersCount++;
                    }
                    qsort(numbers, numbersCount, sizeof(int), compare);
                    median = medianInArray(numbers, numbersCount);
                    average = sum / ((days[j].endingIndex + 1) - days[i].startingIndex);

                    periods[perCount].startingIndex = days[i].startingIndex;
                    periods[perCount].endingIndex = days[j].endingIndex;
                    periods[perCount].y1 = days[i].year;
                    periods[perCount].y2 = days[j].year;
                    periods[perCount].m1 = days[i].month;
                    periods[perCount].m2 = days[j].month;
                    periods[perCount].d1 = days[i].day;
                    periods[perCount].d2 = days[j].day;
                    periods[perCount].average = average;
                    periods[perCount].median = median;
                    periods[perCount].difference = fabs(median - average);
                    periods[perCount].review_count = numbersCount;
                    perCount++;
                }
            }
            double biggestDiff = -1;
            int position = 0, review_count = 0, lastSum = 0;
            for(int i = 0; i < perCount; i++){
                int compareSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                if(periods[i].difference > biggestDiff){
                    biggestDiff = periods[i].difference;
                    lastSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                    review_count = periods[i].review_count;
                    position = i;
                } else if(periods[i].difference == biggestDiff && compareSum > lastSum){
                    biggestDiff = periods[i].difference;
                    lastSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                    review_count = periods[i].review_count;
                    position = i;
                } else if(periods[i].difference == biggestDiff && compareSum == lastSum && periods[i].review_count > review_count){
                    biggestDiff = periods[i].difference;
                    lastSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                    review_count = periods[i].review_count;
                    position = i;
                }
            }
            printf("%d-%d-%d - %d-%d-%d: %lf %d\n", periods[position].y1, periods[position].m1, periods[position].d1,
                   periods[position].y2, periods[position].m2, periods[position].d2, periods[position].average, periods[position].median);
            for(int i = periods[position].startingIndex; i <= periods[position].endingIndex; i++){
                printf("  %d: %s\n", reviews[i].rate, reviews[i].message);
            }
            perCount = 0;
        } else if(sign == '#'){
            int min;
            if(scanf(" %d", &min) != 1 || min < 1){
                error(reviews, days, periods);
                return 1;
            }
            if(min > count){
                printf("Neexistuje.\n");
                continue;
            }
            for(int i = 0; i <= dayCount; i++){
                for(int j = i; j <= dayCount; j++){
                    if(((days[j].endingIndex + 1) - days[i].startingIndex) < min){
                        continue;
                    }
                    int numbers[10000], numbersCount = 0;
                    double average;
                    int median;
                    if(perCapacity == perCount){
                        perCapacity *= 2;
                        Period *temporary = (Period*) realloc(periods, perCapacity * sizeof(Period));
                        periods = temporary;
                    }
                    double sum = 0;
                    for(int k = days[i].startingIndex; k <= days[j].endingIndex; k++){
                        sum += reviews[k].rate;
                        numbers[numbersCount] = reviews[k].rate;
                        numbersCount++;
                    }
                    qsort(numbers, numbersCount, sizeof(int), compare);
                    median = medianInArray(numbers, numbersCount);
                    average = sum / ((days[j].endingIndex + 1) - days[i].startingIndex);

                    periods[perCount].startingIndex = days[i].startingIndex;
                    periods[perCount].endingIndex = days[j].endingIndex;
                    periods[perCount].y1 = days[i].year;
                    periods[perCount].y2 = days[j].year;
                    periods[perCount].m1 = days[i].month;
                    periods[perCount].m2 = days[j].month;
                    periods[perCount].d1 = days[i].day;
                    periods[perCount].d2 = days[j].day;
                    periods[perCount].average = average;
                    periods[perCount].median = median;
                    periods[perCount].review_count = numbersCount;
                    periods[perCount].difference = fabs(median - average);
                    perCount++;
                }
            }
            double biggestDiff = -1;
            int position = 0, review_count = 0, lastSum = 0;
            for(int i = 0; i < perCount; i++){
                int compareSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                if(periods[i].difference > biggestDiff){
                    biggestDiff = periods[i].difference;
                    lastSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                    review_count = periods[i].review_count;
                    position = i;
                } else if(periods[i].difference == biggestDiff && compareSum > lastSum){
                    biggestDiff = periods[i].difference;
                    lastSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                    review_count = periods[i].review_count;
                    position = i;
                } else if(periods[i].difference == biggestDiff && compareSum == lastSum && periods[i].review_count > review_count){
                    biggestDiff = periods[i].difference;
                    lastSum = (periods[i].y2 * 365) + (periods[i].m2 * 30) + periods[i].d2;
                    review_count = periods[i].review_count;
                    position = i;
                }
            }
            printf("%d-%d-%d - %d-%d-%d: %lf %d\n", periods[position].y1, periods[position].m1, periods[position].d1,
                   periods[position].y2, periods[position].m2, periods[position].d2, periods[position].average, periods[position].median);
            perCount = 0;
        } else {
            error(reviews, days, periods);
            return 1;
        }
    }
    free(reviews);
    free(periods);
    free(days);
    return 0;
}
