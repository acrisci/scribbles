#include "libcatfacts.h"
#include "json.hpp"
#include <curl/curl.h>
#include <string>

using namespace std;
using json = nlohmann::json;

//
//  libcurl variables for error strings and returned data

//
//  libcurl write callback function
//

static int writer(char *data, size_t size, size_t nmemb,
                  std::string *writerData)
{
  if(writerData == NULL)
    return 0;

  writerData->append(data, size*nmemb);

  return size * nmemb;
}

//
//  libcurl connection initialization
//

static bool init(CURL *&conn, char *url, std::string *buffer, char *errorBuffer)
{
    CURLcode code;

    conn = curl_easy_init();

    if(conn == NULL) {
        fprintf(stderr, "Failed to create CURL connection\n");
        exit(EXIT_FAILURE);
    }

    code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
    if(code != CURLE_OK) {
        fprintf(stderr, "Failed to set error buffer [%d]\n", code);
        return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_URL, url);
    if(code != CURLE_OK) {
        fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
        return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
    if(code != CURLE_OK) {
        fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
        return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
    if(code != CURLE_OK) {
        fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
        return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, buffer);
    if(code != CURLE_OK) {
        fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
        return false;
    }

    return true;
}

CatFact::CatFact() {
  
}

bool CatFact::doSomething() {
  return true;
}

string CatFact::get() {
    CURL *conn = NULL;
    CURLcode code;
    std::string title;
    std::string buffer;
    char errorBuffer[CURL_ERROR_SIZE];
    char* catfacts_api = "http://catfacts-api.appspot.com/api/facts";

    // Ensure one argument is given

    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Initialize CURL connection

    if(!init(conn, catfacts_api, &buffer, errorBuffer)) {
        fprintf(stderr, "Connection initializion failed\n");
        return NULL;
    }

    // Retrieve content for the URL

    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if(code != CURLE_OK) {
        fprintf(stderr, "Failed to get '%s' [%s]\n", catfacts_api, errorBuffer);
        return NULL;
    }

    auto api_data = json::parse(buffer);

    return api_data["facts"][0];
}
