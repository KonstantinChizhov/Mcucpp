#pragma once
#include <math.h>

namespace Mcucpp
{
	template<class T>
	class Vec3d
	{
	public:
		typedef T Type;
		
		Type  x, y, z;
		
		Vec3d ()
		:x(0), y(0), z(0)
		{
			
		}

		template<class T2>
		Vec3d (T2 v)
		{
			x = y = z = v;
		}

		template<class T2>
		Vec3d (const Vec3d<T2>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}

		template<class T2>
		Vec3d (T2 vx, T2 vy, T2 vz)
		{
			x = vx;
			y = vy;
			z = vz;
		}

		Vec3d& operator = ( const Vec3d& v )
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		template<class T2>
		Vec3d& operator = (T2 f)
		{
			x = y = z = f; 
			return *this; 
		}

		Vec3d operator - () const;
		Vec3d& operator += ( const Vec3d& );
		Vec3d& operator -= ( const Vec3d& );
		Vec3d& operator *= ( const Vec3d& );
		
		template<class T2>
		inline Vec3d<T>& operator*=(T2 v)
		{
			x *= v;
			y *= v;
			z *= v;
			return *this;
		}
		
		template<class T2>
		inline Vec3d<T>& operator /= (T2 v)
		{
			x /= v;
			y /= v;
			z /= v;
			return *this;
		}

		//dot product 
		Type DotProduct(const Vec3d& v)
		{
			return x*v.x + y*v.y + z*v.z;
		}
		
		// cross product
		Vec3d CrossProduct(const Vec3d& v)
		{
			return Vec3d(
					y*v.z - z*v.y,
					z*v.x - x*v.z,
					x*v.y - y*v.x);
		}
		
		Type operator! () const
		{
			return sqrt(x*x + y*y + z*z);
		}

		inline Vec3d RotateX(float a)
		{
			float ca=cos(a), sa=sin(a);
			float zn=z*ca+y*sa;
			float yn=y*ca-z*sa;
			return Vec3d(x, yn, zn);
		}

		inline Vec3d RotateY(float a)
		{
			float ca=cos(a), sa=sin(a);
			float xn=x*ca+z*sa;
			float zn=z*ca-x*sa;
			return Vec3d(xn, y, zn);
		}

		inline Vec3d RotateZ(float a)
		{
			float ca=cos(a), sa=sin(a);
			float xn=x*ca+y*sa;
			float yn=y*ca-x*sa;
			return Vec3d(xn, yn, z);
		}
		
		inline Vec3d Rotate(const Vec3d &v, Type angel)
		{
			Vec3d r=v*Q_rsqrt(v.x*v.x + v.y*v.y + v.z*v.z)*angel, res;
			res=RotateX(r.x);
			res=res.RotateY(r.y);
			res=res.RotateZ(r.z);
			return res;
		}
		
		inline Vec3d Normalize()
		{
			return *this / sqrt(x*x + y*y + z*z);
		}
	};

	
	template<class T>
	inline Vec3d<T> Vec3d<T>::operator- () const
	{
		return Vec3d<T>( -x, -y, -z );
	}

	template<class T>
	inline Vec3d<T> operator+ ( const Vec3d<T>& u, const Vec3d<T>& v )
	{
		return Vec3d<T>( u.x + v.x, u.y + v.y, u.z + v.z ); 
	}

	template<class T>
	inline Vec3d<T> operator- ( const Vec3d<T>& u, const Vec3d<T>& v )
	{
		return Vec3d<T>( u.x - v.x, u.y - v.y, u.z - v.z ); 
	}

	template<class T>
	inline Vec3d<T> operator* ( const Vec3d<T>& u, const Vec3d<T>& v )
	{
		return Vec3d<T>( u.x * v.x, u.y * v.y, u.z * v.z ); 
	}

	template<class T, class T2>
	inline Vec3d<T> operator* ( const Vec3d<T>& u, T2 f )
	{
		return Vec3d<T>( u.x * f, u.y * f, u.z * f ); 
	}

	template<class T, class T2>
	inline Vec3d<T> operator* ( T2 f, const Vec3d<T>& v )
	{
		return Vec3d<T>( f * v.x, f * v.y, f * v.z ); 
	}

	template<class T, class T2>
	inline Vec3d<T> operator/ ( const Vec3d<T>& v, T2 f )
	{
		return Vec3d<T>( v.x / f, v.y / f, v.z / f);
	}

	template<class T>
	inline Vec3d<T> operator/ (const Vec3d<T>& u, const Vec3d<T>& v )
	{
		return Vec3d<T>( u.x / v.x, u.y / v.y, u.z / v.z ); 
	}

	template<class T>
	inline Vec3d<T>& Vec3d<T>::operator+=(const Vec3d<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	template<class T>
	inline Vec3d<T>& Vec3d<T>::operator-=( const Vec3d<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	template<class T>
	inline Vec3d<T>& Vec3d<T>::operator*=(const Vec3d<T>& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
}

