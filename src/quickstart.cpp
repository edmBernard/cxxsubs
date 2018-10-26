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

  void validate() {
    // Validate parsing result : check help options, check rule between option (exclusivity etc ...)
    // cxxopts::Options is accessible with this->options
    // cxxopts::ParseResult is accessible with this->parse_result (warning it's a pointer)
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }
  }

  void exec() {
    // Execute what you want
    // cxxopts::Options is accessible with this->options
    // cxxopts::ParseResult is accessible with this->parse_result (warning it's a pointer)
    std::cout << "command : " << this->get_verbs() << std::endl;
    std::cout << "module : " << (*this->parse_result)["module"].as<std::string>() << std::endl;
  };
};

int main(int argc, char *argv[]) {
  cxxsubs::Verbs<OptionsInit>(argc, argv);
}
