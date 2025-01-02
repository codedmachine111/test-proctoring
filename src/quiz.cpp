#include "../include/quiz.hpp"

void display_quiz(const char *filename, int num_q){
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "cat %s", filename);
    printf("QUIZ QUESTIONS\n");

    if((system(cmd))!=0){
        printf("text.txt file not found in assets directory!\n");
        exit(EXIT_FAILURE);
    }

    printf("\n\n There are %d questions. You need to answer questions as per example given below: \n", num_q);
    printf("\n 1212.. (for Q1, Q2, Q3, Q4..so on) \n");
    printf("\n NOTE: ONLY PRESS ENTER WHEN YOU ARE DONE WRTING YOUR ANSWERS!! \n");
    printf("\n ALL THE BEST! \n");
}