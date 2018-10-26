#include "cxxsubs.hpp"
#include <string>
#include <vector>

class OptionsInit : public cxxsubs::IOptions {
public:
  OptionsInit()
      : cxxsubs::IOptions({"init"}, "Initialise repository") {
    // clang-format off
    this->options.positional_help("[optional args]").show_positional_help();

    this->options.add_options()
      ("m, module", "module name", cxxopts::value<std::string>()->default_value("my_module"))
      ("help", "Print help");
    // clang-format on
  }

  void validate() {
    if (this->parsing_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;
    std::cout << "module : " << (*this->parsing_result)["module"].as<std::string>() << std::endl;
  };
};

int main(int argc, char *argv[]) {
  cxxsubs::Verbs<OptionsInit>(argc, argv);
}
