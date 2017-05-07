#ifndef EZNETBPM_HPP
#define EZNETBPM_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <stdlib.h>


namespace EZNetBPM
{
    enum Type { Unknown, P1, P2, P3, P4, P5, P6 };

    class Image
    {
    
    public:
        // Constructor from raw bit data.
        Image(unsigned int width, unsigned int height, unsigned int bytesPerPixel, unsigned char* bits)
            :   width_(width), height_(height), bytePP_(bytesPerPixel), rangePP_(255), bits_(width*height*bytesPerPixel, static_cast<unsigned char>(0))
        {
            for ( unsigned int n = 0; n < bits_.size(); ++n)
                bits_[n] = *(bits+n);
        } 

        // Constructor from file
        Image(const std::string& filename)
            :   width_(0), height_(0), bytePP_(1), rangePP_(1)
        {
            std::ifstream file(filename.c_str());
            
            // read the header
            std::string line;
            while ( line.empty() )
                line = ReadLineIgnoringComment(file);

            // find out which type it is.
            Type type = P1;
            if ( line.size() >= 2 )
                if ( line[0] == 'P')
                    type = static_cast<Type>(atoi(&line[1]));
                    
            // Get the dimensions.
            std::string width, height, whline;
            while ( whline.empty() )
                whline = ReadLineIgnoringComment(file);
            std::istringstream wh(whline);
            wh >> std::ws >> width >> std::ws >> height;
            width_ = atoi(width.c_str());
            height_ = atoi(height.c_str());

            // Optionally get colour range.
            if ( type != P1 || type != P4 )
            {
                std::string rangeline;
                while ( rangeline.empty() )
                    rangeline = ReadLineIgnoringComment(file);
                rangePP_ = atoi(rangeline.c_str());
            }
            
            if ( type == P3 || type == P6 )
                bytePP_ = 3;
            
            unsigned int bitCount = width_ * height_ * bytePP_;
            bits_ = std::vector<unsigned char>(bitCount, static_cast<unsigned char>(0));
            
            // Then grab the bits.
            if ( type >= static_cast<Type>(P4) ? true : false )
            {
                for ( unsigned int n = 0; n < bitCount; ++n )
                    ReadRAW(file, &bits_[n]);
            }
            else
            {
                for ( unsigned int n = 0; n < bitCount; ++n )
                    ReadASCII(file, &bits_[n]);
            }
        }

        // Write image as PPM
        void Write(const Type& type, const std::string& filename)
        {
            std::ofstream file(filename.c_str(), std::ios::binary);

            // write header magic identifier
            file << 'P' << static_cast<unsigned char>(static_cast<unsigned int>(type+48)) << '\n';

            // write header image dimensions
            file << width_ << ' ' << height_ << '\n';

            // write component range.
            if ( type != P1 && type != P4 )
                file << static_cast<unsigned int>(rangePP_) << '\n';
                
            // write the bits.
            if ( type >= static_cast<Type>(P4) )
            {
                for ( unsigned int n = 0; n < bits_.size(); ++n )
                    file << bits_[n];
            }
            else
            {
                for ( unsigned int n = 0; n < bits_.size(); ++n )
                    file << static_cast<unsigned int>(bits_[n]) << ' ';
            }
        }


        // Generic image accessors
        unsigned int Width() const { return width_; }
        unsigned int Height() const { return height_; }
        unsigned char BytePP() const { return bytePP_; }
        unsigned char RangePP() const { return rangePP_; }

        const std::vector<unsigned char>& Bits() const { return bits_; }
        std::vector<unsigned char>& MutableBits() { return bits_; }

    private:
        std::string ReadLineIgnoringComment(std::ifstream& file)
        {
            std::string line;
            std::getline(file, line);
            std::size_t comment = line.find('#');
            return comment == std::string::npos ? line : line.substr(0, comment);
        }

        void ReadASCII(std::ifstream& file, unsigned char* bit)
        {
            unsigned int i;
            file >> i >> std::ws;
            *bit = static_cast<unsigned char>(i);
        }

        void ReadRAW(std::ifstream& file, unsigned char* bit)
        {
            file >> *bit;
        }

        unsigned int width_;
        unsigned int height_;
        unsigned char bytePP_;
        unsigned char rangePP_;

        std::vector<unsigned char> bits_;
    };

}   // namespace EZNetBPM
    
#endif // EZNETBPM_HPPh