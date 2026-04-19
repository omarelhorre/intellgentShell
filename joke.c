#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#include <string.h>
int main()
{
    const char* API_KEY = getenv("OPENAI_API_KEY");
    if(!API_KEY)
    {
        fprintf(stderr, "Your environement variable is not set.\n");
        fprintf(stderr,"Please set it before running this program\n");
        return -1;
    }
    
    printf("What do you want your joke about? Ex(e.g. computers, cats):");
    char answer[256];
    if(!fgets(answer, sizeof(answer), stdin))
    {
        fprintf(stderr,"failed to read input...\n");
        return -1;
    }

    size_t len = strlen(answer);
    if(len > 0 && answer[len-1] == '\n')
    answer[len-1] = '\n';

    char prompt[1024];
    snprintf(prompt, sizeof(prompt), "Tell me a short, clean, and a family friendly joke about %s."
                        "Please only respond with the joke, and do not add quotation marks or any explanation."
                        "If you can't reply with a friendly family joke say : sorry I could not come up with a joke for that topic", answer);


    char* reply = chatgpt_query(API_KEY, prompt);

    if (reply)
    {
        printf("\n Here is your joke: %s\n",reply);
        free(reply);
    }
    else
    {
        fprintf(stderr, "No jokes for you omar.\n");
    }
    return 0;
}