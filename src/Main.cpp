
#include <iostream>
#include "Config.h"

int main(int arg, char * argv[]) {
    wk::Config config = wk::Config()
            .setIpAddress("1.2.3.4")
            .setPort(8080);

    std::cout << "hi" << std::endl;
}

