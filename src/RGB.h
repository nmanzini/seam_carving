#ifndef RGB_H
#  define RGB_H

#include<fstream>
#include<iostream>

class RGB{
    public:
    unsigned short r = 0;
    unsigned short g = 0;
    unsigned short b = 0;
    unsigned short avg = 0;

    RGB(){};

    RGB(int j,int k,int l){
        r = j;
        g = k;
        b = l;
        avg = (r + g+ b) / 3;
    }

    std::string to_string(){
        std::string out ("");
        out += std::to_string(r) + ' ';
        out += std::to_string(g) + ' ';
        out += std::to_string(b) + '\n';
        return out;
    }


    friend std::ostream& operator<<(std::ostream& os, const RGB& p){
        os << p.r << ' ' << p.g << ' ' << p.b << '\n';
        return os;
    }
};

unsigned short get_grad(unsigned short Left , unsigned short Right)
{
    return (255 / 2) + (Left - Right) / 2;
}

unsigned short get_grad2(unsigned short Left , unsigned short Right)
{
    return abs((Left - Right));
}

#endif
