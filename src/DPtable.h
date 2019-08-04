#ifndef DPTABLE_H
#  define DPTABLE_H

#include <tuple>
#include "Gradient.h"

// image used for dynamic Programming value storage
class DPTable: public Image<std::tuple<int, size_t>>{
    
    // DP table filling based on a gradient image, evaluates the cost of selecting a certain seam and accumualte the minimal
    void fill_table_from_gradient(Gradient &gradient){
        // values for last row.
        size_t last_row = size_y - 1;

        for (size_t i = 0; i< size_x; i++){
            img[last_row][i] = std::tuple(gradient.get_pixel(last_row,i),-1);
        } 

        // start filling from the bottom row.
        for (int j = last_row -1; j >= 0; j--){
            size_t i = 0;
            // first column
            // value for [j][0]
            {
                int local = gradient.get_pixel(j,i);

                int center = std::get<0>(img[j + 1][i]) + local;
                int right = std::get<0>(img[j + 1][i+1]) + local;

                size_t center_id = i;
                size_t right_id = i + 1;

                if (center < right){
                    img[j][i] = std::tuple(center, center_id);
                }else{
                    img[j][i] = std::tuple(right, right_id);
                }
            }

            i++;
            
            // value for [j][1..(size_x - 2)]
            while (i < size_x - 1){

                int local = gradient.get_pixel(j,i);

                int left = std::get<0>(img[j + 1][i-1]) + local;
                int center = std::get<0>(img[j + 1][i]) + local;
                int right = std::get<0>(img[j + 1][i+1]) + local;

                size_t left_id = i - 1;
                size_t center_id = i;
                size_t right_id = i + 1;

                if (left < right && left < center){
                    img[j][i] = std::tuple(left, left_id);
                } else if (right < center){
                    img[j][i] = std::tuple(right, right_id);
                } else {
                    img[j][i] = std::tuple(center, center_id);
                }
                i++;
            }

            // last column
            // value for [j][size_x - 1]
            {
                int local = gradient.get_pixel(j,i);

                int left = std::get<0>(img[j + 1][i-1]) + local;
                int center = std::get<0>(img[j + 1][i]) + local;

                size_t left_id = i - 1;
                size_t center_id = i;

                if (center < left){
                    img[j][i] = std::tuple(center, center_id);
                }else{
                    img[j][i] = std::tuple(left, left_id);
                }
            }
        }
    }

    // return the min value of the starting element of the seam
    size_t find_min(){
        int min = std::get<0>(img[0][0]);
        int min_id = 0;
        for(auto pixel:img[0]){
            size_t i = std::get<1>(pixel);
            int value = std::get<0>(pixel);

            if (value < min){
                // std::cout << value << " "<< i <<std::endl;
                min = value;
                min_id = i;
            }
        }
        // std::cout << min <<" " << min_id << std::endl;
        return min_id;
    }

    public: 

    // find the least important seam
    auto get_seam(){
        std::vector<size_t> seam (size_y);
        // get the index of the min value in the first row
        size_t min = find_min();

        size_t i = min;
        for(size_t j = 0; j < size_y; j++)
        {
            seam[j] = i;
            i = std::get<1>(img[j][i]);
        }
        return seam;
    }

    // create a DP table from an RGB image
    DPTable(Image<RGB> &RGB_img){
        // create a gradient from it
        Gradient gradient(RGB_img);
        // get all the sizes
        size_x = gradient.get_x();
        size_y = gradient.get_y();
        range = gradient.get_range();
        file_name = "DP table";
        img.resize(size_y, std::vector<std::tuple<int, size_t>>(size_x));

        // fill Dynamic Programming table
        fill_table_from_gradient(gradient);
        // save_to_file("dp_table.ppm");
    }

    // put to string for debugging pourposes
    virtual std::string to_string(){
        std::stringstream ss;
        ss <<"P3\n" << this->size_x << ' ' << this->size_y << '\n' << this->range << '\n';
        ss<< "# file name = "<< this->file_name<< std::endl;
        for (auto row : this->img){
            for (auto pixel : row){
                ss << std::get<0>(pixel) << ' ' << std::get<1>(pixel) << '\n';
            }
            ss<< std::endl;
        }
        std::string s = ss.str();
        return s;
    }
};

#endif
