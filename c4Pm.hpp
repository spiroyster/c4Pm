#ifndef C4PM_HPP
#define C4PM_HPP

#include <vector>
#include <string>
#include <functional>

namespace c4Pm
{

enum BPMType { P1, P2, P3, P4, P5, P6 };

class Image
{
public:
    static const unsigned int GREYSCALE = 2;
    static const unsigned int RGB = 3;
    static const unsigned int RGBA = 4;

    unsigned int GetWidth() const { return width_; }
    unsigned int GetHeight() const { return height_; }
    unsigned int GetChannels() const { return channels_; }
    BPMType GetBPMType() const { return type_; }

    const unsigned char& operator[](int n) const { return bits_[n]; }
    unsigned char& operator[](int n) { return bits_[n]; }

    Image(unsigned int width, unsigned int height, unsigned int channels, unsigned char* bits)
        :   width_(width), height_(height), channels_(channels), bits_(width*height*channels, static_cast<unsigned char>(0))
    {
        for ( unsigned int n = 0; n < bits_.size(); ++n)
            bits_[n] = *(bits+n);
    } 

    // Create an image from the file
    Image(const std::string& filename)
        :   width_(0), height_(0), channels_(RGB)
    {
        // Determin the Type by the file extension.


        // Read the header

                                  
    }

    void Write(const BPMType& bpmType, const std::string& filename)
    {
        std::ofstream file(filename.c_str());

        switch (bpmType)
        {
        case P1:
            file << "P1\n" << width_ << " " << height_ << "" 
        }

        
        
        




    }

private:

    unsigned int width_;
    unsigned int height_;
    unsigned int channels_;
    BPMType type_;
    std::vector<unsigned char> bits_;
};

}   // namespace c4Pm


#endif // C4PM_HPP