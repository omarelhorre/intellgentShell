#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#include <string.h>
int main(int argc ,char* argv[])
{
    const char* API_KEY = getenv("OPENAI_API_KEY");
    if(!API_KEY)
    {
        fprintf(stderr, "Your environement variable is not set.\n");
        fprintf(stderr,"Please set it before running this program\n");
        return -1;
    }
    if(argc<2)
    {
        fprintf(stderr, "Expected %s <question>", argv[0]);
        return -1;
    }
    size_t total_len = 0;
    for(int i = 1; i<argc; i++)
    {
        total_len += strlen(argv[i]) + 1;
    }

    char* question = malloc(total_len + 1);
    if(!question)
    {
        fprintf(stderr, "Error allocation");
        return -1;
    }
    question[0] = '\0';
    for(int i = 1; i < argc ; i++)
    {
        strcat(question, argv[i]);
        if(i<argc-1)
        {
            strcat(question, " ");
        }
    }
    const char* prefix = "IF the question is innapropriate"
    ", contains fould language, or cannot be answered"
    ", then please answer with : 'Sorry I cannot answer your question' "
    ". Otherwise, please answer the following question"
    "concsely in plain language: ";
    size_t prompt_length = strlen(prefix) + strlen(question) + 1;
    char* prompt = malloc(prompt_length);
    if(!prompt)
    {
        free(question);
        return -1;
    }
    snprintf(prompt, prompt_length,"%s %s" , prefix,question);
    free(question);
    char* reply = chatgpt_query(API_KEY, prompt);

    if (reply)
    {
        printf("> %s\n",reply);
        free(reply);
    }
    else
    {
        fprintf(stderr, "Failed to get a response, please try again later.\n");
    }
    return 0;
}