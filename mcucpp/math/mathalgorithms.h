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

#include <HiRangeType.h>
#include <template_utils.h>
#include <vec2d.h>
#include <div.h>

namespace Mcucpp
{

	enum class GeometrySide
	{
		Left  =  1,
		Right = -1,
		None  =  0
	};
	
/// Doubled 2D triangle area by 3 points
	template<class T>
	static inline T TriangleArea2(const Vec2d<T> &p1, const Vec2d<T> &p2, const Vec2d<T> &p3)
	{
		T area = ((p1.x - p3.x)*(p2.y - p3.y) - (p2.x - p3.x)*(p1.y - p3.y));
		return area;
	}

/// 2D triangle area by 3 points
	template<class T>
	static inline T TriangleArea(const Vec2d<T> &p1, const Vec2d<T> &p2, const Vec2d<T> &p3)
	{
		return TriangleArea2(p1, p2, p3) / 2;
	}

/// Returns the side of the point relative to ray
/// \param p1 - ray begin.
/// \param p2 - ray end.
/// \param p3 - test point.
	template<class T>
	static inline GeometrySide LinePointSide(
		const Vec2d<T> &p1,
		const Vec2d<T> &p2,
		const Vec2d<T> &p3
		)
	{
		Vec2d<T> perp(p1.y - p2.y, p2.x - p1.x);
		T d = (p3 - p1).DotProduct(perp);
		return d > T(0) ? Left : Right;
	}


	template<class T>
	static inline bool LineSegmentsIntersect(
		const Vec2d<T> &p1,
		const Vec2d<T> &p2,
		const Vec2d<T> &p3, 
		const Vec2d<T> &p4, 
		Vec2d<T> &result
		)
	{
		//???
		Vec2d<T> s1 = p2 - p1;
		Vec2d<T> s2 = p4 - p3;
		Vec2d<T> d  = p1 - p3;

		T divisor = s1.x * s2.y - s1.y * s2.x;
		if(divisor ==  T(0))
			return false;
		T sign = T(1);
		if (divisor < T(0))
		{
			sign = T(-1);
			divisor = -divisor;
		}
		T s = (s1.x * d.y - s1.y * d.x) * sign;

		if (s >= T(0) && s <= divisor)
		{
			T t = (s2.x * d.y - s2.y * d.x) * sign;
			if (t >= T(0) && t <= divisor)
			{
				result.x = p1.x + ((typename HiRangeType<T>::Result)(t) * s1.x) / divisor;
				result.y = p1.y + ((typename HiRangeType<T>::Result)(t) * s1.y) / divisor;
				return true;
			}
		}
		return false;
	}


	static inline bool LineSegmentsIntersect(
		const Vec2d<int32_t> &p1,
		const Vec2d<int32_t> &p2,
		const Vec2d<int32_t> &p3, 
		const Vec2d<int32_t> &p4, 
		Vec2d<int32_t> &result
		)
	{
		Vec2d<int32_t> s1 = p2 - p1;
		Vec2d<int32_t> s2 = p4 - p3;
		Vec2d<int32_t> d  = p1 - p3;

		int32_t divisor = s1.x * s2.y - s1.y * s2.x;
		if(divisor ==  0)
			return false;
		int32_t sign = 1;
		if (divisor < 0)
		{
			sign = -1;
			divisor = -divisor;
		}
		int32_t s = (s1.x * d.y - s1.y * d.x) * sign;

		if (s >= 0 && s <= divisor)
		{
			int32_t t = (s2.x * d.y - s2.y * d.x) * sign;
			if (t >= 0 && t <= divisor)
			{
				result.x = p1.x + div64x32s( (int64_t)t * s1.x, divisor, 0);
				result.y = p1.y + div64x32s( (int64_t)t * s1.y, divisor, 0);
				return true;
			}
		}
		return false;
	}


	template<class T, class Points>
	static inline T GetPolygonArea(const Points &points)
	{
		if (points.size() < 3)
			return 0;

		T res = 0;
		size_t j   = points.size() - 1;
		for (size_t i = 0; i < points.size(); i++)
		{
			T jx = points[j].x;
			T ix = points[i].x;
			T jy = points[j].y;
			T iy = points[i].y;

			res += ( jx + ix) * (jy - iy); 
			j = i;
		}
		return Mcucpp::Util::abs(res) / 2;
	}


	template<class T, class Points>
	static inline Vec2d<T> GetPolygonCenter(const Points &points)
	{
		if(points.size() < 3)
			return Vec2d<T>();
		Vec2d<T> oo = points.front();
		Vec2d<typename HiRangeType<T>::Result> o;
		typename HiRangeType<T>::Result m = 0;
		for (size_t i = 2; i < points.size(); ++i)
		{
			T a = TriangleArea2(points[i-1], points[i], oo);
			Vec2d<typename HiRangeType<T>::Result> avg( points[i-1] + points[i] + oo);
			o += avg * a;
			m += a;
		}
		o /= m * 3;

		return Vec2d<T>(T(o.x), T(o.y));
	}

	template<class T>
	static inline T Determinant3(const T &m)
	{
		return  m[3*0+0] * m[3*1+1] * m[3*2+2] +
				m[3*0+2] * m[3*1+0] * m[3*2+1] +
				m[3*0+1] * m[3*1+2] * m[3*2+0] -
				m[3*0+2] * m[3*1+1] * m[3*2+0] -
				m[3*0+0] * m[3*2+1] * m[3*1+2] -
				m[3*2+2] * m[3*1+0] * m[3*0+1];
	}
}


