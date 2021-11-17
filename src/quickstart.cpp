//
//  cxxsubs
//
//  https://github.com/edmBernard/cxxsubs
//
//  Created by Erwan BERNARD on 23/10/2018.
//
//  Copyright (c) 2018 Erwan BERNARD. All rights reserved.
//  Distributed under the Apache License, Version 2.0. (See accompanying
//  file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
//

#include "cxxsubs.hpp"
#include <string>
#include <vector>

class OptionsInit : public cxxsubs::IOptions {
public:
  OptionsInit()
      : cxxsubs::IOptions({"init"}, "Initialise repository") {
    // Create parser : add option group etc ...
    // cxxopts::Options is accessible with this->options
    this->options.add_options()
      ("m, module", "module name", cxxopts::value<std::string>()->default_value("my_module"))
      ("help", "Print help");
  }

  int validate() {
    // Validate parsing result : check help options, check rule between option (exclusivity etc ...)
    // cxxopts::Options is accessible with this->options
    // cxxopts::ParseResult is accessible with this->parse_result (warning it's a pointer)
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      // We must return FAILURE otherwise exec member will be executed
      return EXIT_FAILURE;
    }
    // If we return Success the exec function will be executed
    return EXIT_SUCCESS;
  }

  int exec() {
    // Execute what you want
    // cxxopts::Options is accessible with this->options
    // cxxopts::ParseResult is accessible with this->parse_result (warning it's a pointer)
    std::cout << "command : " << this->get_verbs() << std::endl;
    std::cout << "module : " << (*this->parse_result)["module"].as<std::string>() << std::endl;
    return EXIT_SUCCESS;
  };
};

int main(int argc, char *argv[]) {
  return cxxsubs::Verbs<OptionsInit>(argc, argv);
}
