#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>
#include <iostream>

struct Tensor {
    std:: vector<float> data;
    int H,W,C;
    
    float at(int h, int w, int c){
        return data[h*W*C + w*C +c];
    }

    float at(int h, int w, int c) const{
        return data[h*W*C + w*C + c];
    }
};

Tensor loadImageAsTensor(const std::string& path, bool grayscale =false){
    cv::Mat img = cv::imread(path,grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR);
    if(img.empty())
        throw std::runtime_error("Failed to load image : " + path);

    if(!grayscale)
        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);

    cv::Mat imgFloat;
    img.convertTo(imgFloat,CV_32F, 1.0/255.0);

    Tensor t;
    t.H = imgFloat.rows;
    t.W = imgFloat.cols;
    t.C = imgFloat.channels();

    t.data.resize(t.H * t.W * t.C);

    std::memcpy(t.data.data(), imgFloat.ptr<float>(),t.data.size()* sizeof(float));

    return t;

}

int main(){
    const std::string path= "/Users/admin/Documents/Projects/C++/test.png";
    try{
        Tensor t = loadImageAsTensor(path,false);
        std::cout<<"Tensor Loaded : [H = "<<t.H
        <<", W= "<<t.W
        <<", C= "<<t.C << "\n";

        std::cout<< "Pixel at (1,1): R="<< t.at(1,1,0)
        <<", G="<< t.at(1,1,1)
        <<", B="<< t.at(1,1,2) << "\n";
    } catch (const std::exception& e){
        std::cerr << "Error: "<< e.what() << std::endl;
        return 1;
    }
    return 0;
}