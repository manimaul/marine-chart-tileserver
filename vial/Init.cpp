#include "vial.h"
#include <folly/init/Init.h>

namespace vial {

    void Init::init(int *argc, char ***argv) {
        std::cout << "vial version: " << VIAL_VERSION << std::endl;
        folly::init(argc, argv);
    }
}