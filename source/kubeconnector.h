#include <curl/curl.h>

#ifndef LANGDICT_H
#define LANGDICT_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* MANUALMODE;
    char* ERRORUSER;
    char* DIRNOTIFY;
    char* DIRSCANNING;
    char* TEMPFILESCREATED;
    char* DELTEMPQUEST;
    char* DELTEMPNO;
    char* DELTEMPYES;
    char* DELTEMPERROR;
    char* NONPRINTABLE;
    char* YAMLPROCESSING;
    char* FOPENERROR;
    char* TEMPFERROR;
    char* FILEPROC;
    char* VARVALUE;
    char* VARFOUNDED;
    char* TFILECREATED;
    char* DIROPENERROR;
    char* PROCFILEQUEST;
    char* PROCFILENO;
    char* TDIROPERROR;
    char* FILEDELETED;
    char* DELFILEERROR;
    char* TEMPDIRDEL;
    char* TEMPDIRDELERROR;
    char* UNSUPPORTEDLANG;
    char* DIRCREATFAIL;
    char* ALLOCATIONERROR;
    char* UNSUPPORTEDMODE;
} Dictionary;

extern Dictionary spa;
extern Dictionary eng;

typedef struct{
    char* USAGETITTLE;
    char* USAGEVERNOSE;
    char* USAGELANGUAJE;
    char* USAGEPATH;
    char* USAGECONFIG;
} UsageDict;

extern UsageDict usspa;
extern UsageDict useng;

void print_usage();

#endif // LANGDICT_H


void applyYamlFile(const char *filePath, const char *apiServerUrl, const char *token) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Content-Type: application/yaml");
        char authHeader[512];
        snprintf(authHeader, sizeof(authHeader), "Authorization: Bearer %s", token);
        headers = curl_slist_append(headers, authHeader);

        curl_easy_setopt(curl, CURLOPT_URL, apiServerUrl);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        FILE *file = fopen(filePath, "rb");
        if (!file) {
            perror("Failed to open file");
            return;
        }

        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READDATA, file);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        fclose(file);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int execute_deploy() {
    //Preview
    const char *apiServerUrl = "https://<KUBERNETES_API_SERVER>/apis/networking.k8s.io/v1/namespaces/default/ingresses";
    const char *token = "<YOUR_TOKEN>";
    const char *filePath = "ingress-nlb.yaml";

    applyYamlFile(filePath, apiServerUrl, token);

    return 0;
}
