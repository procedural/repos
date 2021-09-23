#include "../framework/redgpu_f.h"

#include <stdlib.h> // For exit
#include <iostream> // For std::cout
#include <string>   // For std::string
#include <sstream>  // For std::stringstream

void setup() {
  unsigned index = 1;
  while (1) {
    std::stringstream ss;
    ss << "wget -qO- https://api.github.com/users/redgpu/repos?page=";
    ss << index;

    std::string output;
    {
      char * out = NULL;
      redFSystem(ss.str().c_str(), &out, 0);
      output = out;
      redFFree(out);
    }

    if (1) { // Debug
      std::cout << "[DEBUG]" << std::endl << output << std::endl;
    }

    if (output == "" || output == "[\n\n]\n") {
      break;
    }

    index += 1;
  }

  exit(0);
}

int main() {
  RedFEvents events = {};
  events.setup = setup;
  redFMain(&events, 256, 256, REDF_WINDOW_MODE_WINDOW, 0, 1, 0, 1, 0, 0);
}
