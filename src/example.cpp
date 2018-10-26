
#include "cxxsubs.hpp"
#include <string>
#include <vector>

using iop = cxxsubs::IOptions;

class OptionsShipNew : public cxxsubs::IOptions {
public:
  OptionsShipNew()
      : cxxsubs::IOptions({"ship", "new"}, "Create a new ship") {

    options.positional_help("[optional args]").show_positional_help();

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

    options.positional_help("[optional args]").show_positional_help();

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

    options.positional_help("[optional args]").show_positional_help();

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

    options.positional_help("[optional args]").show_positional_help();

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

// class OptionsInit : public cxxsubs::IOptions {
// public:
//   OptionsInit() try {
//     this->verbs = {"init"};
//     this->options = std::make_unique<cxxopts::Options>("init", "A simple command line test");
//     options.positional_help("[optional args]").show_positional_help();

//     // clang-format off
//     options.add_options()
//       ("help", "Print help")
//       ("w, width", "How wide should it be?",
//         cxxopts::value<int>()->default_value("0"), "WIDTH")
//       ("n, name", "By what name should I be known",
//         cxxopts::value<std::string>(), "NAME")
//       ("d, doit", "Do the thing",
//         cxxopts::value<bool>()->default_value("false"))
//       ("c, command", "which command to run",
//         cxxopts::value< std::vector<std::string>>(), "COMMAND");
//     // clang-format on

//     options.parse_positional({"command"});

//   } catch (const cxxopts::OptionException &e) {
//     std::cout << "Error: parsing options: " << e.what() << std::endl;
//     exit(1);
//   }

//   void validate() {
//     if (this->parsing_result->count("help")) {
//       std::cout << this->options.help({""}) << std::endl;
//       exit(0);
//     }

//     if (this->parsing_result->count("width")) {
//       std::cout << "Error: parsing options: missing width"  << std::endl;
//       exit(1);
//     }

//   }

//   void exec() {
//     std::cout << "debug init" << std::endl;

//     std::cout << "width :" << (*this->parsing_result)["width"].as<int>() << std::endl;
//     std::cout << "name :" << ((*this->parsing_result).count("name") ? (*this->parsing_result)["name"].as<std::string>() : "") << std::endl;
//     std::cout << "doIt :" << (*this->parsing_result)["doit"].as<bool>() << std::endl;

//     if ((*this->parsing_result).count("command")) {
//       for (auto&& i : (*this->parsing_result)["command"].as< std::vector<std::string>>()) {
//         std::cout << "command :" << i << std::endl;
//       }
//     }

//     std::cout << "showHelp :" << (*this->parsing_result).count("help") << std::endl;
//     std::cout << "test :" << 1 << std::endl;
//   };
// };

// class OptionsDevAdd : public cxxsubs::IOptions {
// public:
//   OptionsDevAdd() {
//     this->verbs = {"dev", "add"};
//     this->options = std::make_unique<cxxopts::Options>("toto", "une description");
//     this->options.positional_help("[optional args]").show_positional_help();

//     // clang-format off
//     this->options.add_options()
//         ("m,module", "module name", cxxopts::value<std::string>())
//         ("help", "Print help");
//     // clang-format on
//   }

//   void validate() {
//     if (this->parsing_result->count("help")) {
//       std::cout << this->options.help({""}) << std::endl;
//       exit(0);
//     }
//   };

//   void exec() {
//     std::cout << "debug dev add" << std::endl;
//   };
// };

int main(int argc, char *argv[]) {
  cxxsubs::Verbs<OptionsShipNew, OptionsShipMove, OptionsShipShoot>(argc, argv);
}
