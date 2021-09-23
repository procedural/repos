#include "../framework/redgpu_f.h"
#include "jsmn.h"

#include <stdlib.h> // For malloc, free
#include <string.h> // For memset
#include <stdlib.h> // For exit
#include <iostream> // For std::cout
#include <vector>   // For std::vector
#include <string>   // For std::string
#include <sstream>  // For std::stringstream

std::vector<std::string> getGithubReposUrls(std::string githubProfileName, unsigned jsonTokensMaxCount) {
  std::vector<std::string> reposUrls;

  jsmntok_t * jsonTokens = (jsmntok_t *)malloc(jsonTokensMaxCount * sizeof(jsmntok_t));
  if (jsonTokens == 0) {
    return reposUrls;
  }

  unsigned index = 1;
  while (1) {
    std::stringstream ss;
    ss << "wget -qO- https://api.github.com/users/";
    ss << githubProfileName;
    ss << "/repos?page=";
    ss << index;

    std::string output;
    {
      char * out = NULL;
      redFSystem(ss.str().c_str(), &out, 0);
      output = out;
      redFFree(out);
    }

    if (0) { // Debug
      std::cout << "[DEBUG]" << std::endl << output << std::endl;
    }

    if (output == "" || output == "[\n\n]\n") {
      break;
    }

    memset(jsonTokens, 0, jsonTokensMaxCount * sizeof(jsmntok_t));
    jsmn_parser jsonParser;
    jsmn_init(&jsonParser);
    int jsonTokensCount = jsmn_parse(&jsonParser, output.c_str(), output.length() + 1, jsonTokens, jsonTokensMaxCount);

    for (unsigned i = 0; i < jsonTokensCount; i += 1) {
      jsmntok_t token = jsonTokens[i];
      if (token.type == JSMN_STRING) {
        std::string s = output.substr(token.start, token.end - token.start);
        if (s == "owner") {
          // Skipping
          jsmntok_t nextToken = jsonTokens[i + 1];
          i += 1 + (nextToken.size * 2);
          continue;
        }
        if (s == "html_url") {
          jsmntok_t   nextToken = jsonTokens[i + 1];
          std::string url       = output.substr(nextToken.start, nextToken.end - nextToken.start);
          reposUrls.push_back(url);
        }
      }
    }

    index += 1;
  }

  free(jsonTokens);
  return reposUrls;
}

void setup() {
  std::vector<std::string> reposUrlsREDGPU     = getGithubReposUrls("redgpu", 65536);
  std::vector<std::string> reposUrlsProcedural = getGithubReposUrls("procedural", 65536);

  std::cout << "# [Procedural](http://github.com/procedural)'s Github Repos:" << std::endl << std::endl;
  for (auto & url : reposUrlsREDGPU) {
    std::stringstream ss;
    ss << "git ls-remote ";
    ss << url;
    std::string sha;
    {
      char * out = NULL;
      redFSystem(ss.str().c_str(), &out, 0);
      sha = out;
      redFFree(out);
    }
    std::cout << url   << std::endl;
    std::cout << "```" << std::endl;
    std::cout << sha;
    std::cout << "```" << std::endl << std::endl;
  }
  for (auto & url : reposUrlsProcedural) {
    std::stringstream ss;
    ss << "git ls-remote ";
    ss << url;
    std::string sha;
    {
      char * out = NULL;
      redFSystem(ss.str().c_str(), &out, 0);
      sha = out;
      redFFree(out);
    }
    std::cout << url   << std::endl;
    std::cout << "```" << std::endl;
    std::cout << sha;
    std::cout << "```" << std::endl << std::endl;
  }

  exit(0);
}

int main() {
  RedFEvents events = {};
  events.setup = setup;
  redFMain(&events, 256, 256, REDF_WINDOW_MODE_WINDOW, 0, 1, 0, 1, 0, 0);
}
