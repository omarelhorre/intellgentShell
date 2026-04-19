#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#define BUFFER_SIZE 4096
int main()
{
    const char* API_KEY = getenv("OPENAI_API_KEY");
    if(!API_KEY)
    {
        fprintf(stderr, "Your environement variable is not set.\n");
        fprintf(stderr,"Please set it before running this program\n");
        return -1;
    }
    char task[BUFFER_SIZE];
    printf("Enter a description of your desired shell task:\n >");
    if(!fgets(task,BUFFER_SIZE,stdin))
    {
        printf("Error reading\n");
        return -1;
    }
    char *template = "You are a Linux/Unix shell command assistant"
                        "Given a plain description of a task, output the shell command."
                        "that will accomplish it using bash-compatible syntax"
                        "only output the command nothing else."
                        "if you cannot create the shell command or the description of the task is not appropriate"
                        "please output instead : 'Sorry I cannot create the shell command of this task'. and nothing else.\n\nTask : %s";
    char prompt[BUFFER_SIZE*2];
    snprintf(prompt,sizeof(prompt),template,task);
    char* reply = chatgpt_query(API_KEY, prompt);

    if (reply)
    {
        printf(">: %s\n",reply);
        free(reply);
    }
    else
    {
        fprintf(stderr, "Failed to get a response, please try again later.\n");
    }
    return 0;
}