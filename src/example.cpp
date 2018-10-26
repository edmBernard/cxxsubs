
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

  void validate() {
    if (this->parsing_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }

    if (this->parsing_result->count("name") < 1) {
      std::cout << "Error: parsing options: require atleast one <name>" << std::endl;
      exit(1);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    if ((*this->parsing_result).count("name")) {
      for (auto &&i : (*this->parsing_result)["name"].as<std::vector<std::string>>()) {
        std::cout << "name :" << i << std::endl;
      }
    }
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

  void validate() {
    if (this->parsing_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }

    if (!this->parsing_result->count("x") || !this->parsing_result->count("y")) {
      std::cout << "Error: parsing options: missing <x> or <y>" << std::endl;
      exit(1);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "x :" << (*this->parsing_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parsing_result)["y"].as<int>() << std::endl;
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

  void validate() {
    if (this->parsing_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }

    if (!this->parsing_result->count("name") || !this->parsing_result->count("x") || !this->parsing_result->count("y")) {
      std::cout << "Error: parsing options: missing <name>, <x> or <y>" << std::endl;
      exit(1);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "name :" << (*this->parsing_result)["name"].as<std::string>() << std::endl;
    std::cout << "x :" << (*this->parsing_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parsing_result)["y"].as<int>() << std::endl;
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

  void validate() {
    if (this->parsing_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }

    if (!this->parsing_result->count("x") || !this->parsing_result->count("y")) {
      std::cout << "Error: parsing options: missing <x> or <y>" << std::endl;
      exit(1);
    }

    if (this->parsing_result->count("moored") && this->parsing_result->count("drifting")) {
      std::cout << "Error: parsing options: drifting and moored are exclusived option" << std::endl;
      exit(1);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "x :" << (*this->parsing_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parsing_result)["y"].as<int>() << std::endl;
    std::cout << "moored :" << this->parsing_result->count("moored") << std::endl;
    std::cout << "drifting :" << this->parsing_result->count("drifting") << std::endl;
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

  void validate() {
    if (this->parsing_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }

    if (!this->parsing_result->count("x") || !this->parsing_result->count("y")) {
      std::cout << "Error: parsing options: missing <x> or <y>" << std::endl;
      exit(1);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;

    std::cout << "x :" << (*this->parsing_result)["x"].as<int>() << std::endl;
    std::cout << "y :" << (*this->parsing_result)["y"].as<int>() << std::endl;
  };
};


int main(int argc, char *argv[]) {
  cxxsubs::Verbs<OptionsShipNew, OptionsShipMove, OptionsShipShoot, OptionsMineSet, OptionsMineRemove>(argc, argv);
}
