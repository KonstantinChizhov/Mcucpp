#pragma once
#include "loki/Typelist.h"

template<int __CaseLable, class ResultType>
struct Case
{
    typedef ResultType Result;
    enum{CaseLable = __CaseLable};
};

template<int __CaseLable, int __ResultValue>
struct CaseValue
{
    enum{CaseLable = __CaseLable, Value = __ResultValue};
};

//-----------------------------------------------------------------------------
template<int value, class Cases, class DefaultType = Loki::NullType> struct StaticSwitch;

template<int value, class Head, class Tail, class DefaultType>
struct StaticSwitch<value, Loki::Typelist<Head, Tail>, DefaultType>
{
     typedef typename StaticSwitch<value, Tail, DefaultType>::Result Result;
};

template<int value, class DefaultType>
struct StaticSwitch<value, Loki::NullType, DefaultType>
{
    typedef DefaultType Result;
};

template<int value, class CaseResult, class Tail, class DefaultType>
struct StaticSwitch<value, Loki::Typelist<Case<value, CaseResult>, Tail>, DefaultType>
{
     typedef CaseResult Result;
};


template<int value, class Cases, int DefaultValue = 0> struct StaticSwitchValue;

template<int value, class Head, class Tail,int DefaultValue>
struct StaticSwitchValue<value, Loki::Typelist<Head, Tail>, DefaultValue>
{
     enum{Value =  StaticSwitchValue<value, Tail, DefaultValue>::Value};
};

template<int value, int DefaultValue>
struct StaticSwitchValue<value, Loki::NullType, DefaultValue>
{
   enum{Value = DefaultValue};
};

template<int value, int ResultValue, class Tail, int DefaultValue>
struct StaticSwitchValue<value, Loki::Typelist<CaseValue<value, ResultValue>, Tail>, DefaultValue>
{
      enum{Value = ResultValue};
};

