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
#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <core/Object.h>
#include <vector>
#include <core/FileStream.h>
#include <core/Ref.h>
#include <stdint.h>
namespace graphics
{

	class Image : public core::Object
	{
	public:
		Image(int width, int height, int bytesPerPixel);

		uint8_t* getData();

		int getDataLenInBytes();

		int getWidth();
		int getHeight();
		int getBPP();

		virtual ~Image();

		static Image* loadFromTGA(const std::string& strFileName);
		static Image* loadFromTGA(const char* strFileName);
	private:
		int m_width;
		int m_height;
		int m_bytesPerPixel;
		std::vector<uint8_t> m_data;

		Image();
	};

}



#endif



