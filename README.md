# cxxsubs

Basic C++ Subcommand/Verbs support for command line. It use [cxxopts](https://github.com/jarro2783/cxxopts) for each subcommand parsing.

The design was inspire by this [C# library](https://github.com/commandlineparser/commandline)

## Design


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
naval_fate mine set <x> <y> [--moored|--drifting]
navel_fate mine remove <x> <y>
```
*idea from the [docopt library](http://docopt.org/)*




