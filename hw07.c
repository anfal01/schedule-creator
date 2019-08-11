/*
*  author: Anfal Chaid
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct class{
   int section;
   char subject[5];
   int catalog;
   char name[31];
   char building[5];
   int room;
   char mtgStart[6], mtgEnd[6];
   char Mon[2];
   char Tues[2];
};

struct scheduleClass{
   char subject[5];
   int catalog;
};

/* Here, a sort is created to sort whether or not if the class is Monday Tuesday */
struct class* monTuesSort(struct class *s){
   int i = 0, j = 0;
   struct class *a;
   a = malloc(250 * sizeof(struct class));
   while(i < 250){
      if (strcmp(s[i].Mon, "Y" ) == 0){
         a[j] = s[i];
         j++;
      }
      i++;
   }

   i = 0;
   while(i < 250){
      if (strcmp(s[i].Tues, "Y" ) == 0){
         a[j] = s[i];
         j++;
      }
      i++;
   }
   return a;
}

/* Using a bubble sort to subsort by class start time */
struct class* timeSort(struct class *s){
   int i = 0, j = 0;
   struct class a;

   while (s[i].section != 0) {
      j = 0;
      while (s[j+1].section != 0) {
         if((strcmp(s[j].Mon, "Y") == 0 && strcmp(s[j+1].Mon, "Y") == 0 ) ||
            (strcmp(s[j].Tues, "Y") == 0 && strcmp(s[j+1].Tues, "Y") == 0)
           ) { 
            if (strcmp(s[j].mtgStart, s[j+1].mtgStart) > 0){
               a = s[j+1];
               s[j+1] = s[j];
               s[j] = a;
            }
         } 
         j++;
      }
      i++;
   }
   return s;
}

struct class* courseSort(struct class *s){
   int i, j, k;
   struct class a;

   i = 0;
   while(s[i].section != 0) {
      j = 0;
      while(s[j+1].section != 0) {
         if( (strcmp(s[j].mtgStart, s[j+1].mtgStart) == 0) && 
             (strcmp(s[j].Mon, s[j+1].Mon) == 0 ||
              strcmp(s[j].Tues, s[j+1].Tues) == 0)) {
            if(strcmp(s[j].subject, s[j+1].subject) > 0) {
               a = s[j+1];
               s[j+1] = s[j];
               s[j] = a;
            }
         }
         j++;
      }
      i++;
   }

   i = 0;
   while(s[i].section != 0) {
      j = 0;
      while(s[j+1].section != 0) {
         if(strcmp(s[j].subject, s[j+1].subject) == 0 &&
           (strcmp(s[j].mtgStart, s[j+1].mtgStart) == 0) && 
           (strcmp(s[j].Mon, s[j+1].Mon) == 0 ||
            strcmp(s[j].Tues, s[j+1].Tues) == 0)) {
            if(s[j].catalog > s[j+1].catalog){
               a = s[j+1];
               s[j+1] = s[j];
               s[j] = a;
            }
         }
         j++;
      }
      i++;
   } 
   return s;
}

printSched(struct class *schedule, int a){
   int i;
   printf("***  Monday schedule  ***\n");
   for(i = 0; i < a; i++) 
      if(strcmp(schedule[i].Mon, "Y") == 0)
         printf("%s %5s %d.%03d  %s--%s  %-7s  %s\n", schedule[i].mtgStart, schedule[i].subject, schedule[i].catalog, schedule[i].section, schedule[i].mtgStart, schedule[i].mtgEnd, schedule[i].building, schedule[i].name); 

   printf("\n***  Tuesday schedule  ***\n");
   for(i = 0; i < a; i++) 
      if(strcmp(schedule[i].Tues, "Y") == 0)
         printf("%s %5s %d.%03d  %s--%s  %-7s  %s\n", schedule[i].mtgStart, schedule[i].subject, schedule[i].catalog, schedule[i].section, schedule[i].mtgStart, schedule[i].mtgEnd, schedule[i].building, schedule[i].name); 
}

int main(){
   FILE *fp1 = fopen("fall2019courses.csv", "r");
   FILE *fp2 = fopen("eligible.txt", "r");
   char *line = NULL;
   struct class *s, *schedule;
   struct scheduleClass *e;
   int len = 0, nread, i = 0, j = 0, k = 0, a = 0, b = 0, m = 0;
   char *time1 = "08:00", *time2 = "08:00", str[4];

   s = malloc(250 * sizeof(struct class));
   nread = getline(&line, &len, fp1);
   
   /* Reading in fall2019courses.csv and storing it in a array of struct class */
   while ((nread = getline(&line, &len, fp1)) != -1) {
      s[i].section = atoi(strtok(line, ","));
      strcpy(s[i].subject, strtok(NULL, ","));
      s[i].catalog = atoi(strtok(NULL, ","));
      strcpy(s[i].name, strtok(NULL, ","));
      strcpy(s[i].building, strtok(NULL, ","));
      s[i].room = atoi(strtok(NULL, ","));
      strcpy(s[i].mtgStart, strtok(NULL, ","));
      strcpy(s[i].mtgEnd, strtok(NULL, ","));
      strcpy(s[i].Mon, strtok(NULL, ","));
      strcpy(s[i].Tues, strtok(NULL, "\n"));
      b++;
      i++;
   }

   /* Storing eligible classes */
   len = 0;
   line = NULL;
   i = 0;
   e = malloc(15 * sizeof(struct scheduleClass));
   while((nread = getline(&line, &len, fp2)) != -1) {
     strcpy(e[i].subject, strtok(line, " "));
     e[i].catalog = atoi(strtok(NULL, "\n"));
     i++;
     a++;
   }

   /* Sorting then subsorting accordingly */
   s = monTuesSort(s); 
   s = timeSort(s); 
   s = courseSort(s);

   schedule = malloc(15 * sizeof(struct class));

   m = 0;
   k = 0;
   for(i = 0; i < b; i++){
      for(j = 0; j < a; j++){
         if((strcmp(s[i].mtgStart, time1) >= 0) && (strcmp(s[i].Mon, "Y") == 0) && (strcmp(s[i].subject, e[j].subject) == 0) && (s[i].catalog == e[j].catalog)){
            for(m = 0; m < a; m++)
               if((s[i].catalog == schedule[m].catalog) && (strcmp(s[i].subject, schedule[m].subject) == 0))
                  goto repeat1;
            time1 = s[i].mtgEnd;
            schedule[k] = s[i];
            k++;
         }
         if((strcmp(s[i].mtgStart, time2) >= 0) && (strcmp(s[i].Tues, "Y") == 0) && (strcmp(s[i].subject, e[j].subject) == 0) && (s[i].catalog == e[j].catalog)){
            for(m = 0; m < a; m++)
               if((s[i].catalog == schedule[m].catalog) && (strcmp(s[i].subject, schedule[m].subject) == 0))
                  goto repeat1;
            time2 = s[i].mtgEnd;
            schedule[k] = s[i];
            k++;
         }
         repeat1:
            continue;
      }
   }

   for(i = 0; i < b; i++){
      sprintf(str, " %d", schedule[i].room);
      strcat(schedule[i].building, str);
   }
  
   printSched(schedule, a);
   
   return 0;
}
