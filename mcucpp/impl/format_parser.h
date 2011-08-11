#pragma once
namespace IO
{
	template<class Stream, FormatMode Mode, class FormatStrPtrType>
	void FormatParser<Stream, Mode, FormatStrPtrType>::ProcessFormat()
	{
		if(_formatSrting)
		{
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
								streamsize_t width;
								ptr += Impl::StringToIntDec<streamsize_t>(ptr, width);
								out.width(width);
                            }
                            if(ScanFloatPrecision && *ptr == '.')
                            {
                                ptr++;
                                streamsize_t presc;
                                ptr += Impl::StringToIntDec<streamsize_t>(ptr, presc);
                                out.precision(presc);
                            }
                            if(*ptr == '|')
                                ptr++;
                        }
                    }
					_formatSrting = ptr;
                    return;
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
}
