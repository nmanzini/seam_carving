#ifndef IMAGE_H
#  define IMAGE_H

#include "RGB.h"
#include <vector>
#include <iostream>
#include <sstream>

// general pourpouse Image Class, a vector of vector of templated type.
// it can be used wiht RGB or scalar values or touples.
template<typename t>
class Image
{
    protected:
    std::vector<std::vector< t >> img;
    size_t size_x = 0;
    size_t size_y = 0;
    size_t range = 255;
    std::string file_name;
    public: 
    size_t get_x(){ return size_x;}
    size_t get_y(){ return size_y;}
    size_t get_range(){return range;}
    t get_pixel(size_t i, size_t j){ return img[i][j];}
    auto &get_row(size_t j){return img[j];}
    auto get_img(){return img;}

    friend std::ostream& operator<<(std::ostream& os, const Image<t>& i){
        os << i.to_string();
        return os;
    }
    virtual std::string to_string(){
        std::stringstream ss;
        ss <<"P3\n" << this->size_x << ' ' << this->size_y << '\n' << this->range << '\n';
        ss<< "# file name = "<< this->file_name<< std::endl;
        std::string s = ss.str();
        return s;
    };

    void save_to_file(std::string path){
        std::ofstream output_file(path);
        output_file << this->to_string();
        std::cout << path << " saved to file" << std::endl;
    }
};

#endif