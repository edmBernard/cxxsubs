# cxxsubs

Basic C++ Subcommand/Verbs support for command line. It use [cxxopts](https://github.com/jarro2783/cxxopts) for each subcommand parsing.

The design was inspire by this [C# library](https://github.com/commandlineparser/commandline)

Code is available on github [cxxsubs](https://github.com/edmBernard/cxxsubs)

## Design


## Some interesting feature

- Completion script, you can add special `cxxsubs::CompletionCommand` which will add completion command for bash (cf. [`example.cpp`](src/example.cpp))


## Known limitations

- No Real Help message
- Subcommand can overlap (ex: `ship new`, `ship`). The `ship new` command will also execute `ship` command.



## Quick start Example

1. Create separate option classes for each verb. Class have to work like IOptions.
1. Call cxxsubs::Verbs with all the verb options classes in template parameters.


C++ Class For New Verb:

```cpp
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
```

C++ Main:

```cpp
int main(int argc, char *argv[]) {
  cxxsubs::Verbs<OptionsInit>(argc, argv);
}
```


## Complete Example

A more complete example can be found in [`example.cpp`](src/example.cpp) file.
The Example aim to produce the following command

```bash
naval_fate ship new <name>...
naval_fate ship shoot <x> <y>
naval_fate ship move <name> <x> <y>
naval_fate mine set <x> <y> [--moored|--drifting]
navel_fate mine remove <x> <y>
```
*idea from the [docopt library](http://docopt.org/)*

## Requirements

- [cxxsubs](https://github.com/edmBernard/cxxsubs) depends on [cxxopts](https://github.com/jarro2783/cxxopts).
- The only build requirement is a C++ compiler that supports C++14 regular expressions and some variadic template feature (std::index_sequence). For example GCC >= 5.5 works with c++14 options.

## Documentation and examples generation

```bash
mkdir build
cd build
cmake ..
make
make docs
```

Example programs (`example` and `quickstart`) are located in build folder. The Documentaion will be generated in `doc/html/index.html`.
