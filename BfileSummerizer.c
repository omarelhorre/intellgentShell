#include <stdio.h>
#include <stdlib.h>
#include "chatgpt.h"
#include <string.h> 
char* read_file(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(!fp) return NULL;

    fseek(fp,0,SEEK_END);
    long length = ftell(fp);
    rewind(fp); 
    char* buffer = malloc(length+1);
    if(!buffer) {fclose (fp); return  NULL;}
    fread(buffer,1,length,fp);
    buffer[length] = '\n';
    fclose(fp);
    return buffer;

}
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"expected %s <textfile>\n", argv[0]);
        return -1;
    }

    const char* API_KEY = getenv("OPENAI_API_KEY");
    if(!API_KEY)
    {
        fprintf(stderr, "Your environement variable is not set.\n");
        fprintf(stderr,"Please set it before running this program\n");
        return -1;
    }
    char* file_contents = read_file(argv[1]);
    if(!file_contents) {fprintf(stderr,"Error, could not read file"); return -1; free(file_contents);}
    const char* instruction = "Summarize the following text in 3-5 concise bullet points:\n\n";
    size_t prompt_len = strlen(instruction) + strlen(file_contents) + 1;
    char* prompt = malloc(prompt_len);
    if(!prompt)
    {
        fprintf(stderr, "ERROR: memory allocation");
        free(file_contents);
        return -1;
    }
    strcpy(prompt,instruction);
    strcat(prompt, file_contents);
    char* reply = chatgpt_query(API_KEY, prompt);
    if (reply)
    {
        printf("summary: %s\n",reply);
        free(reply);
    }
    else
    {
        printf("Failed to get a response, please try again later.\n");
        free(file_contents);
        free(prompt);
        return -1; 
    }
    free(file_contents);
    free(prompt);
    return 0;
}