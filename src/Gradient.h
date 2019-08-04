#ifndef GRADIENT_H
#  define GRADIENT_H
#include <thread>

#include "thread-pool.h"
#include "Image.h"

Thread_pool<std::vector<unsigned short>> thread_pool(std::thread::hardware_concurrency());

// image that represend the gradient of the cost of the image
class Gradient: public Image<unsigned short>{
    private:

    std::vector<unsigned short> make_row (std::vector<RGB> source){
        unsigned short prev = source[1].avg;
        unsigned short curr;

        std::vector<unsigned short> grad_row(size_x);

        for (size_t i = 0; i< size_x; i++)
        {
            curr = source[i].avg;
            grad_row[i] = get_grad2(prev,curr);
            prev = curr;
        }
        return grad_row;
    }

    void horizontal_gradient_make_thread_pool(Image<RGB> img_rgb){
        // update the gradient row sing the thread pool
        std::vector<std::future<std::vector<unsigned short>>> f_v(size_y);
        for (size_t j = 0; j < size_y ; j++){
            auto row = img_rgb.get_row(j);
            f_v[j] = thread_pool.enqueue([=](){
                return make_row(row);
            });
        }
        for (size_t j = 0; j < size_y ; j++){
            // prints the id of the task
            img[j] = f_v[j].get();
        }
    }
    
    public:
    Gradient(Image<RGB> &img_rgb){
        // initialize a gradient image
        size_x = img_rgb.get_x();
        size_y = img_rgb.get_y();
        range = img_rgb.get_range();

        img.resize(size_y);
        horizontal_gradient_make_thread_pool(img_rgb);
    }

    virtual std::string to_string(){
        // converts a gradient image into a black and white image (used for debug)
        std::stringstream ss;
        ss <<"P3\n" << this->size_x << ' ' << this->size_y << '\n' << this->range << '\n';
        ss<< "# file name = "<< this->file_name<< std::endl;
        for (auto row : this->img){
            for (auto pixel : row){
                ss << pixel << ' ' << pixel << ' ' << pixel<< '\n';
            }
            ss<< std::endl;
        }
        std::string s = ss.str();
        return s;
    }
};


#endif
