#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>

struct Tensor{
    std::vector<float> data;
    int H,W,C;

    float at(int h, int w, int c){
        return data[h*W*C + w*C +c];
    }

    float at(int h, int w, int c) const{
        return data[h*W*C + w*C +c];
    }
};

Tensor loadImageAsTensor(const std::string& path, bool grayscale = false){
    cv::Mat image = cv::imread(path, grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR);
    if(image.empty()){
        throw std::runtime_error("Failed to load Image " + path);
    }

    if(!grayscale){
        cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    }

    cv::Mat imageFloat;
    image.convertTo(imageFloat, CV_32F, 1.0/255.0);

    Tensor t;
    t.H = imageFloat.rows;
    t.W = imageFloat.cols;
    t.C = imageFloat.channels();

    t.data.resize(t.H * t.W * t.C);

    std::memcpy(t.data.data(), imageFloat.ptr<float>(), t.data.size()* sizeof(float));

    return t;

}

void displayImage(const std::string& path, bool grayscale= false){
    cv::Mat img = cv::imread(path, grayscale? cv::IMREAD_GRAYSCALE: cv::IMREAD_COLOR);
    if(img.empty()){
        throw std::runtime_error("Failed to load image: " + path);
    }
    cv::imshow("Image", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main(){
    const std::string path ="/Users/admin/Documents/Projects/C++/test.png";
    try{
        Tensor t = loadImageAsTensor(path, false);
        std::cout << "Tensor Loaded : [H = "<<t.H
        <<", W= " << t.W
        <<", C= " << t.C << "]\n";

        std::cout<< "Pixel at (1,1): R="<< t.at(1,1,0)
        << ", G= "<< t.at(1,1,1)
        << ", B= "<< t.at(1,1,2)<<"\n";
        displayImage(path,false);
    } catch( const std::exception& e){
        std::cerr << "Error: "<< e.what() << std::endl;
        return 1;
    }
    return 0;

}