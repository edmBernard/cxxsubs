
#include "cxxsubs.hpp"

class OptionsInit : public cxxsubs::IOptions {
public:
    OptionsInit() {
        this->verbs = {"init"};
        this->options = std::make_unique<cxxopts::Options>("init", "initialise repository");
        // clang-format off
        this->options->positional_help("[optional args]").show_positional_help();

        this->options->add_options()
            ("m,module", "module name", cxxopts::value<std::string>())
            ("help", "Print help");
        // clang-format on
    }

    void validate() {
        if (this->parsing_result->count("help")) {
            std::cout << this->options->help({""}) << std::endl;
            exit(0);
        }
    }

    void exec() {
        std::cout << "debug init" << std::endl;
    };
};

class OptionsDevAdd : public cxxsubs::IOptions {
public:
    OptionsDevAdd() {
        this->verbs = {"dev", "add"};
        this->options = std::make_unique<cxxopts::Options>("toto", "une description");
        // clang-format off
        this->options->positional_help("[optional args]").show_positional_help();

        this->options->add_options()
            ("m,module", "module name", cxxopts::value<std::string>())
            ("help", "Print help");
        // clang-format on
    }

    void validate() {
        if (this->parsing_result->count("help")) {
            std::cout << this->options->help({""}) << std::endl;
            exit(0);
        }
    };

    void exec() {
        std::cout << "debug dev add" << std::endl;
    };
};

int main(int argc, char *argv[]) {
    std::cout << "debug1" << std::endl;
    OptionsInit myInitOptions;
    OptionsDevAdd myDevAddOptions;
    std::cout << "myInitOptions.match(argc, argv) :" << myInitOptions.match(argc, argv) << std::endl;
    std::cout << "debug1.1" << std::endl;
    myInitOptions.parse(argc, argv);
    std::cout << "myDevAddOptions.match(argc, argv) :" << myDevAddOptions.match(argc, argv) << std::endl;
    myDevAddOptions.parse(argc, argv);

    std::cout << "debug2" << std::endl;
    cxxsubs::Verbs<OptionsInit, OptionsDevAdd> my_verbs;
    // my_verbs.parse(argc, argv);
    my_verbs.parse();
    std::cout << "debug3" << std::endl;
}
