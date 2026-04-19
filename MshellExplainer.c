#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#include <string.h>
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
    char input[BUFFER_SIZE];
    while(1)
    {
    printf("Enter shell command(s) (type '/q' to exit)\n >");
    if(!fgets(input,BUFFER_SIZE,stdin))
    {
        printf("Error reading\n");
        return -1;
    }
    size_t len = strlen(input);
    if(len > 0 && input[len-1] == '\n')
    {
        input[len-1] = '\0';
    }

    if(strcmp(input,"/q") == 0)
    {
        printf("End session.\n");
        break;
    }

    char prompt[BUFFER_SIZE * 2];
    snprintf(prompt, sizeof(prompt), "Explain in simple terms what the following shell commands do"
                                                        "If the command is relatively simple with only a few arguments "
                                                    "please keep the explanation brief."
                                                "If the command is complex, first provide and overall summary for what the commands do at a high level."
                                                "Then break it down step by step"
                                            "Explain each major part such as pipelines,subsitutions, loops or commands"
                                            "as seprate bullet points, so that the explanation covers the entire command."
                                        "if you cannot provide an explanation because the command text is inappropriate"
                                    "please reponde with : 'sorry I cannot provide an explanation for this command(s).'"
                                "and nothing more. If the command(s) conatain inacurracies, please address this in your explanation.\n\nCommand(s) : %s \n", input);
    char* explanation = chatgpt_query(API_KEY, prompt);
    if(explanation)
    {
        printf("Explanation: %s",explanation);
    }
    else {
    {
        fprintf(stderr, "Error text generation, please try again later\n");
    }
    }
    printf("\n");
    }
    return 0;
}