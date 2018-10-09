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

#include <vec2d.h>
#include <array.h>
#include <ray.h>
#include <template_utils.h>
#include <mathalgorithms.h>

namespace Mcucpp
{

	template<class T, size_t MaxSize>
	class Polygon
	{
		typedef Vec2d<T> Vec;
		typedef Mcucpp::Containers::FixedArray<MaxSize, Vec> ArrayT;
		ArrayT    _points;
		mutable T _area;
		Vec       _p1, _p2;
	public:
		Polygon()
			:_points(),
			_area(0)

		{
		}
		

		explicit Polygon(T top, T left, T bottom, T right)
			:_points()
		{
			_p1 = Vec(left, top);
			_p2 = Vec(right, bottom);
			_points.push_back(_p1);
			_points.push_back(Vec(right, top));
			_points.push_back(_p2);
			_points.push_back(Vec(left, bottom));
			_area = GetPolygonArea<T>(_points);
		}


		bool IntersectBounds(const Ray<T> &ray)
		{
			int side1 = ray.PointSide(_p1);
			int side2 = ray.PointSide(_p2);
			if(side1 != side2)
				return true;
			side2 = ray.PointSide(Vec(_p1.x, _p2.y));
			if(side1 != side2)
				return true;
			side2 = ray.PointSide(Vec(_p2.x, _p1.y));
			if(side1 != side2)
				return true;
			return false;

		}

		bool CutWith(const Ray<T> &ray, Polygon &result)
		{
			bool intersect = false;
			if (_points.size() < 3)
				return intersect;

			if(!IntersectBounds(ray))
				return intersect;

			result.Clear();

			int left = (int)ray.side;

			int side0, side1;
			Vec tmp;
			Vec v0 = _points.back(),
			    v1;

			side0 = ray.PointSide(v0);
			bool v1Push = false;
			for (size_t i = 0; i < _points.size(); i++)
			{
				v1 = _points[i];
				side1 = ray.PointSide(v1);
				if (side0 + side1 == 0 )
				{
					intersect = true;
					if (ray.Intersect(v0, v1, tmp))
						result._points.push_back(tmp);
					else
					{
						v1Push = true;
					}
				}
				if (side1 != -left || v1Push)
				{
					result._points.push_back(v1);
					v1Push = false;
				}
				v0 = v1;
				side0 = side1;
			}
			if(intersect)
				result.UpdateBounds();
			return intersect;
		}

		T GetArea()const
		{
			if(_area == T(0))
				_area = GetPolygonArea<T>(_points);
			return _area;
		}

		Vec GetCenter()const
		{
			return GetPolygonCenter<T>(_points);
		}

		Vec &operator [](int index)
		{
			return _points[index];
		}

		const Vec &operator [](int index) const
		{
			return _points[index];
		}
		

		size_t Size() const
		{
			return _points.size();
		}

		void Clear()
		{
			_points.clear();
			_area = 0;
		}

		void UpdateBounds()
		{
			_p1 = _p2 = _points.front();
			for (size_t i = 1; i < _points.size(); i++)
			{
				if(_points[i].x < _p1.x)
					_p1.x = _points[i].x;
				if(_points[i].y < _p1.y)
					_p1.y = _points[i].y;
				if(_points[i].x > _p2.x)
					_p2.x = _points[i].x;
				if(_points[i].y > _p2.y)
					_p2.y = _points[i].y;
			}
		}

		void AddPoint(T x, T y)
		{
			Vec p(x, y);
			if(_points.size() == 0)
			{
				_p1 = _p2 = p;
			}

			if(p.x < _p1.x)
				_p1.x = p.x;
			if(p.y < _p1.y)
				_p1.y = p.y;
			if(p.x > _p2.x)
				_p2.x = p.x;
			if(p.y > _p2.y)
				_p2.y = p.y;

			_points.push_back(p);
		}

		void* operator new(size_t)throw();
		void operator delete(void*);
	};
}
