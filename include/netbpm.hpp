#ifndef NETBPM_HPP
#define NETBPM_HPP

#include <fstream>
#include <sstream>
#include <vector>

#define NETBPM_BITMASK_BIT_0 1  
#define NETBPM_BITMASK_BIT_1 2  
#define NETBPM_BITMASK_BIT_2 4  
#define NETBPM_BITMASK_BIT_3 8  
#define NETBPM_BITMASK_BIT_4 16 
#define NETBPM_BITMASK_BIT_5 32  
#define NETBPM_BITMASK_BIT_6 64  
#define NETBPM_BITMASK_BIT_7 128 

#define NETBPM_BITSPERBYTE 8

class netbpm
{
public:

	unsigned int width() const { return width_; }						// width of the image
    unsigned int height() const { return height_; }						// height of the image
	unsigned int channels() const { return channels_; }					// number of channels
	unsigned int range() const { return range_; }						// range each pixel value can have
	unsigned int bpp() const { return bpp_; }							// bits per pixel
	unsigned int count() const { return width_ * height_; }				// number of pixels
	unsigned int stride() const { return bpp_ == 1 ? 1 : bpp_ / NETBPM_BITSPERBYTE; }	// stride of a pixel
	
	// default constructor...
	netbpm()
		: width_(0), height_(0), channels_(0), bpp_(0), range_(0)
	{
	}

	// constructor from raw image data...
	netbpm(unsigned int width, unsigned int height, unsigned int channels, unsigned int bpp, const unsigned char* bits)
		: width_(width), height_(height), channels_(channels), bpp_(bpp)
	{
		unsigned int numberOfBytes = 0;
		if (bpp_ == 1)
		{
			range_ = 1;
			
			numberOfBytes = count() / NETBPM_BITSPERBYTE;
			if (count() % NETBPM_BITSPERBYTE != 0)
				++numberOfBytes;

			if (bits)
				readPBM(bits, numberOfBytes);
			else
				bits_ = std::vector<unsigned char>(numberOfBytes, static_cast<unsigned char>(0));
		}
		else if (bpp_ == 8 || bpp_ == 24)
		{
			range_ = 255;
			numberOfBytes = count() * (bpp_ / 8);
			bits_ = std::vector<unsigned char>(numberOfBytes, static_cast<unsigned char>(0));
			if (bits)
				std::memcpy(&bits_[0], bits, numberOfBytes);
		}
		else
			throw std::exception("netbpm bpp not supported.");
	}

	// constructor from ASCII syntax...
	netbpm(const std::string& str)
	{
		std::istringstream iss(str);
		read(iss);
	}

	// read from a stream (binary or ascii)...
	friend std::istream& operator>>(std::istream& is, netbpm& v)
	{
		v.read(is);
		return is;
	} 

	// write to stream (binary only)...
	friend std::ostream& operator<<(std::ostream& os, netbpm& v)
	{
		os << v.header(false);
		if (v.bpp_ == 1)
		{
			std::vector<unsigned char> buffer(v.PBMBufferSize(), static_cast<unsigned char>(0));
			os.write(&buffer.front(), buffer.size());
		}
		else
			os.write(reinterpret_cast<char*>(&v.bits_[0]), v.count() * v.stride());
		return os;
	}

	// index of xy pixel in raw byte array...
	unsigned int pixel(unsigned int x, unsigned int y) const
	{
		return ((y * width_) + x) * stride();
	}

	// accessor for raw byte...
	const unsigned char* operator[](unsigned int pixel) const
	{
		return &bits_[pixel * stride()];
	}
	
	// output to ASCII syntax...
	std::string ascii() const
	{
		std::ostringstream oss;
		oss << header(true);

		std::string seperator(" ");
		if (bpp_ == 1)
			seperator.clear();

		for (unsigned int h = 0; h < height_; ++h)
		{
			for (unsigned int w = 0; w < width_; ++w)
			{
				for ( unsigned int c = 0; c < channels_; ++c )
					oss << static_cast<unsigned int>(bits_[((width_ * h) + w)+c]) << seperator;
			}
			oss << '\n';
		}
		return oss.str();
	}

	void flipH()
	{
		std::vector<unsigned char> newBits;
		


		bits_.swap(newBits);
	}

	void flipV()
	{
		std::vector<unsigned char> newBits;



		bits_.swap(newBits);
	}

	void rotate90()
	{
		std::vector<unsigned char> newBits;



		bits_.swap(newBits);
	}

	void rotate270()
	{
		std::vector<unsigned char> newBits;



		bits_.swap(newBits);
	}

	// helper to get the file extension...
	std::string extension() const
	{
		if (channels_ == 1 && bpp_ == 1)
			return ".pbm";
		if (channels_ == 1 && bpp_ == 8)
			return ".pgm";
		if (channels_ == 3 && bpp_ == 24)
			return ".ppm";
		throw std::exception("netbpm format not supported.");
	}


private:
    unsigned int width_;
    unsigned int height_;
    unsigned int channels_;			// number of channels...
	unsigned int bpp_;				// bits per pixel...
	unsigned int range_;

    std::vector<unsigned char> bits_;


	std::string header(bool ascii) const
	{
		std::ostringstream oss;

		if (bpp_ == 1)
			oss << ascii ? "P1" : "P4";
		if (bpp_ == 8)
			oss << ascii ? "P2" : "P5";
		if (bpp_ == 24)
			oss << ascii ? "P3" : "P6";

		oss << "# this file was created with https://github.com/spiroyster/netbpm\n";

		oss << width_ << " " << height_ << '\n';

		if (bpp_ == 8 || bpp_ == 24)
			oss << range << '\n';

		return oss.str();
	}

	std::string readLine(const std::string& line)
	{
		// remove any comments...
		std::string result;
		std::size_t commentItr = line.find_first_of('#');
		if (commentItr != std::string::npos)
			result = line.substr(0, commentItr);
		else
			result = line;

		result.erase(std::remove(result.begin(), result.end(), ' '),
			result.end());

		return result;
	}

	unsigned int bufferSizePBM() const
	{
		assert(bpp_ == 1);

		unsigned int bytesPerRow = width_ / 8;

		// pad per row...
		if (width_ % 8 != 0)
			++bytesPerRow;

		return bytesPerRow * height_;
	}

	void readPBM(const unsigned char* buffer, unsigned int sizeInBytes)
	{
		bits_.reserve(count());
		for (unsigned int n = 0; n < sizeInBytes; ++buffer, ++n)
		{
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_0) == NETBPM_BITMASK_BIT_0 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_1) == NETBPM_BITMASK_BIT_1 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_2) == NETBPM_BITMASK_BIT_2 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_3) == NETBPM_BITMASK_BIT_3 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_4) == NETBPM_BITMASK_BIT_4 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_5) == NETBPM_BITMASK_BIT_5 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_6) == NETBPM_BITMASK_BIT_6 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
			bits_.push_back((*buffer & NETBPM_BITMASK_BIT_7) == NETBPM_BITMASK_BIT_7 ? static_cast<unsigned char>(1) : static_cast<unsigned char>(0));
		}
	}

	std::vector<unsigned char> writePBM()
	{



	}

	void readASCII()
	{



	}

		
	void read(std::istream& stream)
	{
		// read til the first endline...

		// get the magic P1, P2, P3 are ASCII ... the rest Binary
		std::string header;
		while (header.empty())
		{
			std::getline(stream, header);
			header = readLine(header);
		}

		// we then keep ignoring comments...
		std::string dimensions;
		while (dimensions.empty())
		{
			std::getline(stream, dimensions);
			dimensions = readLine(dimensions);
		}

		/*{
			char* tok = 0;
			tok = strtok(&dimensions.front(), " \t\n");
			unsigned int argc = 0;
			while (tok)
			{
				if (argc == 0)
					width_ = atoi(tok);
				else if (argc == 1)
					height_ = atoi(tok);
				else if (argc == 2)
					range_ = atoi(tok);
				else
					tok = 0;
			}
		}*/


		if (header == "P1" || header == "P4")
		{
			channels_ = 1;
			bpp_ = 1;

			if (header == "P1")
			{

				// read ascii...

				// this ascii may or may not be ws seperated!


			}
			else
			{
				std::vector<char> buffer(bufferSizePBM(), static_cast<char>(0));
				stream.read(&buffer.front(), buffer.size());

				// create the data bits from the buffer...
				//readPBM(&buffer.front(), buffer.size());
			}


		}
		else if (header == "P2" || header == "P3" || header == "P5" || header == "P6")
		{
			if (!range_)
			{
				std::string rangeSyntax;
				while (rangeSyntax.empty())
				{
					std::getline(stream, rangeSyntax);
					rangeSyntax = readLine(rangeSyntax);
				}

				std::istringstream rangeBuff(rangeSyntax);
				rangeBuff >> std::ws >> range_ >> std::ws;
			}

			if (header == "P2" || header == "P3")
			{

			}
			else
			{

			}

			// read the bits into memory...
			//stream.read(static_cast<char*>(&bits_.front()), count() * (bpp_ / 8));
		}

	}

};

#endif // NETBPM_HPP