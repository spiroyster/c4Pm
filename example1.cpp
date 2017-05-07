#include "EZNetBPM.hpp"

int main(int argc, char** argv)
{

    // Load in the PPM image
    EZNetBPM::Image bang("bang_raw.ppm");

    // invert the texels.
    // for ( std::vector<unsigned char>::iterator itr = bang.MutableBits().begin(); 
    //     itr != bang.MutableBits().end(); ++itr )
    // {
    //     *itr = static_cast<unsigned char>(255 - *itr);
    // }
    
    // Save out as PGM image
    bang.Write(EZNetBPM::P6, "output.ppm");

    return 0;
}