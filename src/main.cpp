
#include "Image.h"
#include "Ppm.h"
#include <iostream>
#include <string> 

int main(int argc, char const *argv[])
{
    if (argc == 3){
        PPM img(argv[1]);
        size_t n;
        try{ n = std::stoi(argv[2]);
        }catch ( std::invalid_argument) {
            std::cout << "Not a valid number of columns" << std::endl;
            exit(1);
        }
        img.delete_colums(n);
        img.save_to_file("output.ppm");
    }
    else if (argc == 2){
        PPM img(argv[1]);
        img.delete_colums(img.get_x() / 2);
        img.save_to_file("output.ppm");
    }
    else{
        std::cout << "usage: SeamCarving <image> \nusage: SeamCarving <image> <columns> \n  image   : a P3 ppm image \n  columns : the number of columns to remove, if not included removes half" << std::endl;
    }
    return 0;
}