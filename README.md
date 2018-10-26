# cxxsubs

Lightweight C++ command line Subcommand (Verbs) Parser it use [cxxopts](https://github.com/jarro2783/cxxopts) for each subcommand parsing
The design was inspire by this [C# library](https://github.com/commandlineparser/commandline)

## Design


## Quick Start Examples

1. Create separate option classes for each verb. Class have to work like IOptions.
1. Call cxxsubs::Verbs with all the verb options classes in template parameters.


C++ Class For New Verb:

```cpp
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
```

C++ Main:

```cpp
int main(int argc, char *argv[]) {
    cxxsubs::Verbs<OptionsInit> my_cli_parser;
    my_cli_parser.parse();
}
```


