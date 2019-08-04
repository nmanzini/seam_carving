#ifndef PPM_H
#  define PPM_H

#include "DPtable.h"


class PPM: public Image<RGB>{
    private:
    // delete the least important seam
    void delete_column(){
        DPTable dp_table(*this);
        auto seam = dp_table.get_seam();
        size_t j = 0;
        for(auto i:seam){
            img[j].erase(img[j].begin()+i);
            j++;
        }
        size_x -= 1;
    }

    public:
    // reads in a file path to make a rgb image
    PPM(std::string path){
        std::ifstream file(path,std::ios::binary);
        std::string buffer;

        if(!file.good())
        {
            std::cout<<"File does not exist"<< std::endl;
            exit(1);
        }
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            std::cout<<"File empty"<< std::endl;
            exit(1);
        }

        file_name = path;

        // remove first line, not important in our efforts
        std::getline(file, buffer);

        // read Horizontal size of image size_x
        try{
            file >> buffer;
            size_x = std::stoi (buffer);

            // read Vertical size of image size_x
            file >> buffer;
            size_y = std::stoi(buffer);

            // std::cout<< "image size = " << size_x<<" x "<<size_y<< std::endl;

            // read maximum rgb value
            file >> buffer;
            range = std::stoi(buffer);
        }catch ( std::invalid_argument) {
            std::cout << "Not a valid number in header of ppm file" << std::endl;
            exit(1);
        }
        std::cout << "Initial file resolution: " << size_x << " x " << size_y << std::endl;
        // resize the image vector to fit the pixels
        img.resize(size_y, std::vector<RGB>(size_x));


        // counting how many pixels we have seen
        size_t n = 0;
        size_t i,j;
        std::string buffer2;
        std::string buffer3;
        // int r,g,b;
        while (file >> buffer && n < size_x *size_y ){
            unsigned short r,g,b;
            // initializes i and j coordinates based on m
            i = n % size_x;
            j = n / size_x;
            
            if (!(file >> buffer2)){
                throw std::invalid_argument( "received wrong number of rgb values" );
            }
            if (!(file >> buffer3)){
                throw std::invalid_argument( "received wrong number of rgb values" );
            }
            
            try{
                r = std::stoi(buffer);
                g = std::stoi(buffer2);
                b = std::stoi(buffer3);
            }catch ( std::invalid_argument) {
                std::cout << "Not a valid RGB value" << std::endl;
                exit(1);
            }

            img[j][i] = RGB(r,g,b);
            
            n++;
        }
    };

    // delete n colums from the image
    void delete_colums(size_t n){
        if (n > size_x){
            n = size_x - 1;
        }
        std::cout<<"Removing " << n << " columns." << std::endl;
        for(size_t i = 0; i < n; i++)
        {
            delete_column();
        }
        std::cout << "Final file resolution: " << size_x << " x " << size_y << std::endl;
    }

    // convert PPM to string
    virtual std::string to_string(){
        std::stringstream ss;
        ss <<"P3\n" << this->size_x << ' ' << this->size_y << '\n' << this->range << '\n';
        ss<< "# original file name = "<< this->file_name<< std::endl;
        for (auto row : this->img){
            for (auto pixel : row){
                ss<< pixel;
            }
            ss<< std::endl;
        }
        std::string s = ss.str();
        return s;
    }

};
#endif
