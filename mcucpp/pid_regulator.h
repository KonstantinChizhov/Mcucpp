//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2014
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

#include <template_utils.h>

#pragma once
namespace Mcucpp
{
	template<class T>
	class PidRegulator
	{
	public:
		PidRegulator(T kp, T ki, T kd, T scale = 1);
		void SetMin(T value);
		void SetMax(T value);
		void SetInput(T value);
		void SetOutput(T value);
		void FeedBack(T feedback);
		T Output();
		void SetOutputScale(T value);
	private: 
		void ClipOutput();
		T _output;
		T _kp, _ki, _kd;
		T _err[3];
		T _errInt;
		T _targetValue;
		T _clipMin, _clipMax;
		T _scale;
	};
	
	template<class T>
	PidRegulator<T>::PidRegulator(T kp, T ki, T kd, T scale)
		:_output(0),
		_kp(kp), _ki(ki), _kd(kd),
		_errInt(0),
		_targetValue(0), 
		_clipMin(Util::MinValue<T>::value), _clipMax(Util::MaxValue<T>::value),
		_scale(scale)
	{
		_err[0] = _err[1] = _err[2] = 0;
	}
	
	template<class T>
	void PidRegulator<T>::SetMin(T value)
	{
		_clipMin = value;
		ClipOutput();
	}
	
	template<class T>
	void PidRegulator<T>::SetMax(T value)
	{
		_clipMax = value;
		ClipOutput();
	}
	
	template<class T>
	void PidRegulator<T>::SetInput(T value)
	{
		_targetValue = value;
	}
	
	template<class T>
	void PidRegulator<T>::SetOutput(T value)
	{
		_output = value;
	}
	
	template<class T>
	void PidRegulator<T>::FeedBack(T feedback)
	{
		_err[2] = _err[1];
		_err[1] = _err[0];
		_err[0] = _targetValue - feedback;
		_errInt += _err[0];
		
		T ep = _kp * (_err[0]);
		T ei = _ki * _errInt;
		T ed = _kd * (_err[2] - _err[0]) / 2;
		
		T output = (ep + ei + ed);
		_output = output / _scale;
		ClipOutput();
	}
	
	template<class T>
	T PidRegulator<T>::Output()
	{
		return _output;
	}
	
	template<class T>
	void PidRegulator<T>::ClipOutput()
	{
		if(_output < _clipMin)
			_output = _clipMin;
		if(_output > _clipMax)
			_output = _clipMax;
	}

	template<class T>
	void PidRegulator<T>::SetOutputScale(T value)
	{
		_scale = value;
	}
}
