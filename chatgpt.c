#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <curl/curl.h>
#include "string.h"
#include "chatgpt.h"
char* chatgpt_query(const char* API_KEY, const char* prompt)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", "gpt-5");

    cJSON *messages = cJSON_CreateArray();

    cJSON *msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "role", "user");
    cJSON_AddStringToObject(msg, "content", prompt);

    cJSON_AddItemToArray(messages, msg);

    cJSON_AddItemToObject(root, "messages", messages);

    char* json_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if(!json_data) return NULL;

    CURL* curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    //initialize curl
    curl_global_init(CURL_GLOBAL_ALL); 
    curl = curl_easy_init();
    if(!curl)
    {
        free(json_data);
        return NULL;
        
    }
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header),"Authorization; Bearer %s",API_KEY);
    headers = curl_slist_append(headers,auth_header);
    headers = curl_slist_append(headers,"Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/models");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,json_data); //send the json data
    res = curl_easy_perform(curl); //perform the action and returns the result
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl); 
    curl_global_cleanup();
    free(json_data);
    return NULL;
}