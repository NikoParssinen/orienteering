#include <iostream>

#include "routes.hh"
#include "parser.hh"
#include "cli.hh"

int main()
{
    std::shared_ptr<Routes> routes(new Routes);
    std::cout << "Input> ";
    std::string input;
    getline(std::cin, input);
    if ( !read_input_from_file(input, routes) ){
        return EXIT_FAILURE;
    }
    Cli cli(routes);
    while( cli.exec_prompt() ){ }
    return EXIT_SUCCESS;
}
