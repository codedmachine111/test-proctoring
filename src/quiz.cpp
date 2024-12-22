#include "../include/quiz.hpp"

void display_quiz(const char *filename, int num_q){
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "cat %s", filename);
    printf("QUIZ QUESTIONS\n");
    system(cmd);

    printf("\n\n There are %d questions. You need to answer questions as per example given below: \n", num_q);
    printf("\n 1->1 (for Q1 Option 1) \n");
    printf("\n NOTE: ONLY PRESS ENTER WHEN YOU ARE DONE WRTING YOUR ANSWERS!! \n");
    printf("\n ALL THE BEST! \n");
}