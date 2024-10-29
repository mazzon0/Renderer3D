#include <string>
#include <iostream>
#include "SceneLoader.h"

/*
    r3d_imp bin/text filename_with_postfix out_filename_without_postfix
*/

int main(int argc, char* argv[]) {
    if(argc<4) {
        std::cout << "ERROR: not enough arguments" << std::endl;
        return -1;
    }

    // Loading
    SceneLoader loader;
    std::cout << "loading" << std::endl;
    loader.load(argv[2]);
    if(!loader.successfullyLoaded()) {
        std::cout << "ERROR: failed to load the scene" << std::endl;
        return -1;        
    }

    // Saving
    std::cout << argv[1] << std::endl;
    if(strcmp(argv[1], "bin") == 0) {
        std::cout << "saving in binary format" << std::endl;
        loader.saveBin(std::string(argv[3]) + ".sb3d");
    }
    else if(strcmp(argv[1], "text") == 0) {
        std::cout << "saving in text format" << std::endl;
        loader.saveText(std::string(argv[3]) + ".st3d");
    }
    else {
        std::cout << "ERROR: invalid argument 1, must be \'bin\' or \'text\', provided: " << argv[1] << std::endl;
    }
}