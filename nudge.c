#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#include <time.h>
int main()
{
    srand((unsigned int) time(NULL));
    const char* API_KEY = getenv("OPENAI_API_KEY");
    if(!API_KEY)
    {
        fprintf(stderr, "Your environement variable is not set.\n");
        fprintf(stderr,"Please set it before running this program\n");
        return -1;
    }

    const char* base_prompt[] = {"Give me a simple information in less than 25 words about", "give me something new I can learn in less than 25 words in"};
    const char* modifiers[] = {"DevOps field,", "Software Engineering field,", "AI field,"};
    const char* ending = "Tell me only the information, do not surround it with quotes. " ;
    char prompt[256];
    int base = rand() % 2;
    int modifier = rand() % 2;
    sprintf(prompt, "%s %s %s", base_prompt[base], modifiers[modifier], ending );
    printf("\n%s\n\n\n",prompt);
    char* reply = chatgpt_query(API_KEY, prompt);

    if (reply)
    {
        printf("ChatGPT says: %s\n",reply);
        free(reply);
    }
    else
    {
        fprintf(stderr, "Failed to get a response, please try again later.\n");
        return -1;
    }
    return 0;
}