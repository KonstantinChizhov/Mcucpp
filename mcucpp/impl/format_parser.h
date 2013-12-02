//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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
	template<class Stream, FormatMode Mode, class FormatStrPtrType>
	void FormatParser<Stream, Mode, FormatStrPtrType>::ProcessFormat()
	{
		ClearFmt();
		FormatStrPtrType ptr = _formatSrting;
		while(true)
		{
			if(*ptr == '%')
			{
				ptr++;
				if(*ptr != '%')
				{
					if(*ptr == '|' && ScanFlags)
					{
						bool isFlag=true;
						do{
							ptr++;
							typename Stream::fmtflags flags, mask;

							if(*ptr == '+')
							{
								mask = flags = Stream::showpos;
							}
							else if(*ptr == '#')
							{
								mask = flags = Stream::showbase | Stream::boolalpha;
							}
							else if(*ptr == 'x')
							{
								flags = Stream::hex;
								mask = Stream::basefield;
							}
							else if(*ptr == 'o')
							{
								flags = Stream::oct;
								mask = Stream::basefield;
							}
							else if(*ptr == '0')
							{
								out.fill('0');
								flags = Stream::internal;
								mask = Stream::adjustfield;
							}
							else if(*ptr == '-')
							{
								out.fill(' ');
								flags = Stream::left;
								mask = Stream::adjustfield;
							}
							else
							{
								isFlag = false;
							}

							if(isFlag)
							{
								out.setf(flags, mask);
							}
						}while(isFlag);
						if(ScanFieldWidth)
						{
							out.width(StringToIntDec<streamsize_t>(ptr));
						}
						if(ScanFloatPrecision && *ptr == '.')
						{
							ptr++;
							out.precision(StringToIntDec<streamsize_t>(ptr));
						}
						if(*ptr == '|')
							ptr++;
					}
					_formatSrting = ptr;
					return;
				}
			}
			if(*ptr == '\0')
			{
				_formatSrting = 0;
				return;
			}
			out.put(*ptr);
			ptr++;
		}
	}
}

