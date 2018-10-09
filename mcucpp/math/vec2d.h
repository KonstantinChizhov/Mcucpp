//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************


#pragma once

namespace Mcucpp
{
	template<class T>
	class Vec2d
	{
	public:
		T x, y;

		explicit Vec2d(T _x = T(0), T _y = T(0))
			: x(_x), y(_y)
		{
		}

		template<class T2>
		explicit Vec2d(const Vec2d<T2> &right)
			: x(right.x), y(right.y)
		{
		}

		template<class T2>
		Vec2d operator + (const Vec2d<T2> &right)const
		{
			return Vec2d(x + right.x, y + right.y);
		}

		template<class T2>
		Vec2d operator - (const Vec2d<T2> &right)const
		{
			return Vec2d(x - right.x, y - right.y);
		}

		template<class T2>
		Vec2d operator * (const Vec2d<T2> &right)const
		{
			return Vec2d(x * right.x, y * right.y);
		}

		template<class T2>
		Vec2d operator / (const Vec2d<T2> &right)const
		{
			return Vec2d(x / right.x, y / right.y);
		}

		template<class T2>
		Vec2d operator * (T2 scalar)const
		{
			return Vec2d(x * scalar, y * scalar);
		}

		template<class T2>
		Vec2d operator / (T2 scalar)const
		{
			return Vec2d(x / scalar, y / scalar);
		}

		template<class T2>
		Vec2d& operator += (const Vec2d<T2> &right)
		{
			x += right.x;
			y += right.y;
			return *this;
		}

		template<class T2>
		Vec2d& operator -= (const Vec2d<T2> &right)
		{
			x -= right.x;
			y -= right.y;
			return *this;
		}

		template<class T2>
		Vec2d& operator *= (const Vec2d<T2> &right)
		{
			x *= right.x;
			y *= right.y;
			return *this;
		}

		template<class T2>
		Vec2d& operator /= (const Vec2d<T2> &right)
		{
			x /= right.x;
			y /= right.y;
			return *this;
		}

		template<class T2>
		Vec2d& operator /= (T2 scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		template<class T2>
		Vec2d& operator *= (T2 scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vec2d operator -(void) const
		{
			return vec2d(-x, -y);
		}


		template<class T2>
		T DotProduct(const Vec2d<T2> &right)const
		{
			return x * right.x + y * right.y;
		}
	};
}

