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
#ifndef _REF_H_
#define _REF_H_
#include <es_assert.h>
namespace core
{

template < class Type >
class Ref
{
public:
	inline Ref( const Ref& obj );
	inline Ref( );
	inline Ref( Type* p );
	inline Ref( int p );

	inline ~Ref<Type>();

	
	inline Ref<Type>& operator=( const Ref<Type>& o );
	inline Ref<Type>& operator=( Type* o );
	inline Ref<Type>& operator=( int o );
	
	inline Type* operator->();

	
#if !defined(SWIG)
	inline operator Type*();
#endif	
	
	inline Type* ptr() const;
	inline const Type* operator ->() const;
	inline bool operator ==( const Type* const pObj ) const;
	inline bool operator !=( const Type* const pObj ) const;
	inline bool operator ==( const Ref<Type>& obj ) const;
	inline bool operator !=( const Ref<Type>& obj ) const;
	inline bool operator !( ) const;
	inline Type& operator* () const;

private:
	// Member variables
	Type*		m_ptr;

};

template < class Type >
inline bool operator ==( Type* lhs, Ref<Type> rhs );

template < class Type >
inline bool operator !=( Type* lhs, Ref<Type> rhs );

#include "Ref.inl"

}

#endif


