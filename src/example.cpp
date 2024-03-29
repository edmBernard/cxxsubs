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

using iop = cxxsubs::IOptions;

class OptionsShipNew : public cxxsubs::IOptions {
public:
  OptionsShipNew()
      : cxxsubs::IOptions({"ship", "new"}, "Create a new ship") {

    options.positional_help("<name>").show_positional_help();

    // clang-format off
    options.add_options()
      ("help", "Print help")
      ("n, name", "ship name",
        cxxopts::value<std::vector<std::string>>(), "<name>")
      ;
    // clang-format on

    options.parse_positional({"name"});
  }

  int validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      return EXIT_FAILURE;
    }

    if (this->parse_result->count("name") < 1) {
      std::cout << "Error: parsing options: require atleast one <name>" << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  int exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    if ((*this->parse_result).count("name")) {
      for (auto &&i : (*this->parse_result)["name"].as<std::vector<std::string>>()) {
        std::cout << "name :" << i << std::endl;
      }
    }
    return EXIT_SUCCESS;
  };
};

class OptionsShipShoot : public cxxsubs::IOptions {
public:
  OptionsShipShoot()
      : cxxsubs::IOptions({"ship", "shoot"}, "Shoot foe or not") {

    options.positional_help("<x> <y>").show_positional_help();

    // clang-format off
    options.add_options()
      ("help", "Print help")
      ("x", "x coordinate",
        cxxopts::value<int>(), "<x>")
      ("y", "y coordinate",
        cxxopts::value<int>(), "<y>")
      ;
    // clang-format on

    options.parse_positional({"x", "y"});
  }

  int validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      return EXIT_FAILURE;
    }

    if (!this->parse_result->count("x") || !this->parse_result->count("y")) {
      std::cout << "Error: parsing options: missing <x> or <y>" << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  int exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "x :" << (*this->parse_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parse_result)["y"].as<int>() << std::endl;
    return EXIT_SUCCESS;
  };
};

class OptionsShipMove : public cxxsubs::IOptions {
public:
  OptionsShipMove()
      : cxxsubs::IOptions({"ship", "move"}, "Move one of your ship") {

    options.positional_help("<name> <x> <y>").show_positional_help();

    // clang-format off
    options.add_options()
      ("help", "Print help")
      ("n, name", "ship name",
        cxxopts::value<std::string>(), "<name>")
      ("x", "x coordinate",
        cxxopts::value<int>(), "<x>")
      ("y", "y coordinate",
        cxxopts::value<int>(), "<y>")
      ;
    // clang-format on

    options.parse_positional({"name", "x", "y"});
  }

  int validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      return EXIT_FAILURE;
    }

    if (!this->parse_result->count("name") || !this->parse_result->count("x") || !this->parse_result->count("y")) {
      std::cout << "Error: parsing options: missing <name>, <x> or <y>" << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

  }

  int exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "name :" << (*this->parse_result)["name"].as<std::string>() << std::endl;
    std::cout << "x :" << (*this->parse_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parse_result)["y"].as<int>() << std::endl;
    return EXIT_SUCCESS;
  };
};

class OptionsMineSet : public cxxsubs::IOptions {
public:
  OptionsMineSet()
      : cxxsubs::IOptions({"mine", "set"}, "Put a mine in the field") {

    options.positional_help("<x> <y>").show_positional_help();

    // clang-format off
    options.add_options()
      ("help", "Print help")
      ("x", "x coordinate",
        cxxopts::value<int>(), "<x>")
      ("y", "y coordinate",
        cxxopts::value<int>(), "<y>")
      ("moored", "add moored mine",
        cxxopts::value<bool>())
      ("drifting", "add drifting mine",
        cxxopts::value<bool>())
      ;
    // clang-format on

    options.parse_positional({"x", "y"});
  }

  int validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      return EXIT_FAILURE;
    }

    if (!this->parse_result->count("x") || !this->parse_result->count("y")) {
      std::cout << "Error: parsing options: missing <x> or <y>" << std::endl;
      return EXIT_FAILURE;
    }

    if (this->parse_result->count("moored") && this->parse_result->count("drifting")) {
      std::cout << "Error: parsing options: drifting and moored are exclusived option" << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  int exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "x :" << (*this->parse_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parse_result)["y"].as<int>() << std::endl;
    std::cout << "moored :" << this->parse_result->count("moored") << std::endl;
    std::cout << "drifting :" << this->parse_result->count("drifting") << std::endl;
    return EXIT_SUCCESS;
  };
};

class OptionsMineRemove : public cxxsubs::IOptions {
public:
  OptionsMineRemove()
      : cxxsubs::IOptions({"mine", "remove"}, "Remove a mine") {

    options.positional_help("<x> <y>").show_positional_help();

    // clang-format off
    options.add_options()
      ("help", "Print help")
      ("x", "x coordinate",
        cxxopts::value<int>(), "<x>")
      ("y", "y coordinate",
        cxxopts::value<int>(), "<y>")
      ;
    // clang-format on

    options.parse_positional({"x", "y"});
  }

  int validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      return EXIT_FAILURE;
    }

    if (!this->parse_result->count("x") || !this->parse_result->count("y")) {
      std::cout << "Error: parsing options: missing <x> or <y>" << std::endl;
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  int exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "x :" << (*this->parse_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parse_result)["y"].as<int>() << std::endl;
    return EXIT_SUCCESS;
  };
};

int main(int argc, const char * argv[]) {
  return cxxsubs::Verbs<cxxsubs::CompletionCommand, OptionsShipNew, OptionsShipMove, OptionsShipShoot, OptionsMineSet, OptionsMineRemove>(argc, argv);
}
