// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <graphics/Image.h>
#include <core/Object.h>
#include <vector>
#include <core/FileStream.h>
#include <core/Ref.h>
#include <stdint.h>
namespace graphics
{


	Image::Image(int width, int height, int bytesPerPixel)
		: Object()
		, m_width(width)
		, m_height(height)
		, m_bytesPerPixel(bytesPerPixel)
	{
		m_data.resize(m_width*m_height*m_bytesPerPixel);
	}

	Image::~Image()
	{
	}

	uint8_t* Image::getData()
	{
		return &m_data[0];
	}

	int Image::getDataLenInBytes()
	{
		return (int)m_data.size();
	}

	int Image::getWidth()
	{
		return m_width;
	}
	int Image::getHeight()
	{
		return m_height;
	}
	int Image::getBPP()
	{
		return m_bytesPerPixel;
	}

	Image* Image::loadFromTGA(const std::string& strFileName)
	{
		return loadFromTGA(strFileName.c_str());
	}

	Image* Image::loadFromTGA(const char* strFileName)
	{
		struct TARGA_HEADER
		{
			uint8_t   IDLength, ColormapType, ImageType;
			uint8_t   ColormapSpecification[5];
			uint16_t XOrigin, YOrigin;
			uint16_t ImageWidth, ImageHeight;
			uint8_t   PixelDepth;
			uint8_t   ImageDescriptor;
		};

		core::Ref<core::FileStream> s = new core::FileStream(strFileName, core::FileStream::READ_ONLY);

		static TARGA_HEADER header;
		if (sizeof(header) != s->read(&header, sizeof(header)))
		{
			return 0;
		}

		Image* img = new Image(header.ImageWidth, header.ImageHeight, header.PixelDepth / 8);

		if (img->getDataLenInBytes() != s->read(img->getData(), img->getDataLenInBytes()))
		{
			delete img;
			return 0;
		}

		// Convert the image from BGRA to RGBA
		uint8_t* p = img->getData();
		for (int y = 0; y < header.ImageHeight; y++)
		{
			for (int x = 0; x < header.ImageWidth; x++)
			{
				uint8_t temp = p[2]; p[2] = p[0]; p[0] = temp;
				p += header.PixelDepth / 8;
			}
		}

		return img;
	}
}


