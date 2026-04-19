#include <endian.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#include <string.h>
#define BUFFER_SIZE 4096
void strip_newLine(char* str)
{
    size_t len = strlen(str);
    if(len > 0 && str[len-1] == '\n')
    {
        str[len-1] = '\0';
    }
}
int main()
{
    int i = 0;
    const char* API_KEY = getenv("OPENAI_API_KEY");
    if(!API_KEY)
    {
        fprintf(stderr, "Your environement variable is not set.\n");
        fprintf(stderr,"Please set it before running this program\n");
        return -1;
    }

    char topic[BUFFER_SIZE];
    char user_input[BUFFER_SIZE];
    printf("Welcome to the interactive Quiz Game!\n");
    printf("Enter a quiz topic: ");
    if(!fgets(topic,BUFFER_SIZE,stdin))
    {
        fprintf(stderr, "\n Error reading topic.\n");
        return -1;
    }
    strip_newLine(topic);


    char* previous_questions = malloc(1);
    if(!previous_questions)
    {
        fprintf(stderr, "\n Error storing previous\n");
        return 1;
    }
    previous_questions[0] = '\0';
    size_t total_previous_buffer = 1;
            while(1)
            {
                const char* question_prompt_structure = "Generate a short, clear"
            "quiz question about this topic : %s."
            "Ensure it is different from the questions typically asked and"
            "cover a unique fact or concept. The question should not be too "
            "easy or obvious, or too difficult either."
            "the question should not be one of the following questions"
            "OR a variation of one of the following questions which have been"
            "previously asked : %s\n\n";
            char* question_prompt;
            size_t question_prompt_size = strlen(topic) + strlen(previous_questions) + strlen(question_prompt_structure) + 1;
            question_prompt = malloc(question_prompt_size);
            if(!question_prompt)
            {
                fprintf(stderr, "Error allocation question \n");
                free(previous_questions);
                return -1;
            }
            snprintf(question_prompt,question_prompt_size,question_prompt_structure,topic,previous_questions);

            char * question = chatgpt_query(API_KEY, question_prompt);
            free(question_prompt);
            if(!question)
            {
            fprintf(stderr, "failed to generate question.\n"
            "Session terminated with error message");
            free(previous_questions);
            return -1;

            }

            total_previous_buffer += strlen(question) + 2;
            char * ptr = realloc(previous_questions, total_previous_buffer);
            if(!ptr)
            {
                fprintf(stderr,"\n Error storing previous question. \n");
                free(question);
                free(previous_questions);
                return -1;
            }
            previous_questions = ptr;
            strcat(previous_questions,"\n\n");
            strcat(previous_questions,question); 

                        while(1)
                        {
                        printf("\n\n Question: %s \n\n",question);
                        printf("Enter your answer: ");
                        if(!fgets(user_input,BUFFER_SIZE,stdin))
                        {
                            fprintf(stderr, "\nError reading answer.\n"
                            "Session terminated with error message");
                            free(question);
                            free(previous_questions);
                            return -1;

                        }
                        strip_newLine(user_input);
                        char judge_prompt[BUFFER_SIZE];
                        snprintf(judge_prompt,BUFFER_SIZE,"Question: %s\nUser answer : %s\n Respond with only" 
                            "'Correct' or 'Incorrect' and no other text or punctuation marks.",question,user_input);

                            char * result = chatgpt_query(API_KEY, judge_prompt);
                            if(!result)
                            {
                                fprintf(stderr,"\nError judging answer.\n");
                                free(question);
                                free(previous_questions);
                                return -1;
                            }

                            strip_newLine(result);

                            if(strcmp(result,"Correct") == 0)
                            {
                                printf("\n You got it correct!\n");
                                i++;
                                free(question);
                                free(result);
                                break;
                            }
                            else {
                                printf("\nincorrect\n");
                                printf("Enter  'skip' to skip your question or anything else to retry: ");
                                if(!fgets(user_input, BUFFER_SIZE, stdin))
                                {
                                    fprintf(stderr, "error reading your choice\n");
                                    free(previous_questions);
                                    return -1;
                                }
                                strip_newLine(user_input);
                                if(strcmp(user_input, "skip") == 0) 
                                {   free(question);
                                    free(result);
                                    break;
                                }
                                else {
                                free(result);
                                }
                            }
                        }
                printf("\n Enter '/q' to exit or enter anything else to continue: ");
                if(!fgets(user_input, BUFFER_SIZE, stdin))
                {
                    fprintf(stderr,"\n Error reading input.\n");
                    free(previous_questions);
                    return -1;
                }
                strip_newLine(user_input);
               if(strcmp(user_input, "/q") == 0)
               {
                printf("score: %d\n",i);
                free(previous_questions);
                printf("Exiting\n");
                break;
               }
            }
    return 0;
}