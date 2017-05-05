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
#ifndef ENGINE_REFCOUNTER_H
#define ENGINE_REFCOUNTER_H

#define SHOW_LEAKS

namespace core
{
	class Object;

    class RefCounter
    {
    public:
        RefCounter()
            : refs(0)
        {
        }


        ~RefCounter()
        {
#if defined(SHOW_LEAKS)
            if( refs != 0 )
            {
				printf("[%s] %d Memory leaks detected!", __FUNCTION__, refs);
#if defined(MEMORY_LEAK_DEBUGGING)
                for( size_t i=0; i<m_objects.size(); ++i )
                {
                    ERROR_PRINT_FUNC(m_objectNames[i].c_str());
                }
#endif
            }
            else
            {
				printf("No memory leaks detected!");
            }
#if defined(ASSERT_ON_LEAKS)
            assert( refs == 0 );
#endif
#endif
        }

	
		void add(Object* o, const char* const name)
        {
#if defined(MEMORY_LEAK_DEBUGGING)
            m_objects.push_back(o);
            m_objectNames.push_back(name);
#else
			(void)name;
			(void)o;
#endif
            ++refs;
        }

		void release(core::Object* o)
        {
#if defined(MEMORY_LEAK_DEBUGGING)
            size_t index = 0;
			
			for( ; index<m_objects.size(); ++index )
			{
				if( m_objects[index] == o )
				{
					break;
				}
			}

			m_objects.erase( m_objects.begin() + index );
			m_objectNames.erase( m_objectNames.begin() + index );
#else
			(void)o;
#endif
            --refs;
        }

    private:
#if defined(MEMORY_LEAK_DEBUGGING)
        std::vector<Object*> m_objects;
        std::vector<eastl::string> m_objectNames;
#endif
        int refs;
	public:
	
		
    };

   
}

extern core::RefCounter refs;






#endif

