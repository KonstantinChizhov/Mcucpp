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

//////////////////////////////////////////////////////////////////////////
// DECLARE_ENUM_OPERATIONS is used to declare enum bitwise operations
// to use enum type as a type safe flags
//////////////////////////////////////////////////////////////////////////

#define DECLARE_ENUM_OPERATIONS(ENUM_NAME)                               \
inline ENUM_NAME                                                         \
operator|(ENUM_NAME left, ENUM_NAME right)                               \
{ return ENUM_NAME(static_cast<int>(left) | static_cast<int>(right)); }  \
                                                                         \
inline ENUM_NAME                                                         \
operator&(ENUM_NAME left, ENUM_NAME right)                               \
{ return ENUM_NAME(static_cast<int>(left) & static_cast<int>(right)); }  \
                                                                         \
inline ENUM_NAME                                                         \
operator^(ENUM_NAME left, ENUM_NAME right)                               \
{ return ENUM_NAME(static_cast<int>(left) ^ static_cast<int>(right)); }  \
                                                                         \
inline ENUM_NAME                                                         \
operator~(ENUM_NAME left)                                                \
{ return ENUM_NAME(~static_cast<int>(left)); }                           \
                                                                         \
inline const ENUM_NAME&                                                  \
operator|=(ENUM_NAME& left, ENUM_NAME right)                             \
{ return left = left | right; }                                          \
                                                                         \
inline const ENUM_NAME&                                                  \
operator&=(ENUM_NAME& left, ENUM_NAME right)                             \
{ return left = left & right; }                                          \
                                                                         \
inline const ENUM_NAME&                                                  \
operator^=(ENUM_NAME& left, ENUM_NAME right)                             \
{ return left = left ^ right; }                                          \


#define DECLARE_ENUM_OPERATIONS_TAMPLATE(ENUM_NAME, TEMPLATE)            \
TEMPLATE                                                                 \
inline ENUM_NAME                                                         \
operator|(ENUM_NAME left, ENUM_NAME right)                               \
{ return ENUM_NAME(static_cast<int>(left) | static_cast<int>(right)); }  \
                                                                         \
TEMPLATE                                                                 \
inline ENUM_NAME                                                         \
operator&(ENUM_NAME left, ENUM_NAME right)                               \
{ return ENUM_NAME(static_cast<int>(left) & static_cast<int>(right)); }  \
                                                                         \
TEMPLATE                                                                 \
inline ENUM_NAME                                                         \
operator^(ENUM_NAME left, ENUM_NAME right)                               \
{ return ENUM_NAME(static_cast<int>(left) ^ static_cast<int>(right)); }  \
                                                                         \
TEMPLATE                                                                 \
inline ENUM_NAME                                                         \
operator~(ENUM_NAME left)                                                \
{ return ENUM_NAME(~static_cast<int>(left)); }                           \
                                                                         \
TEMPLATE                                                                 \
inline const ENUM_NAME&                                                  \
operator|=(ENUM_NAME& left, ENUM_NAME right)                             \
{ return left = left | right; }                                          \
                                                                         \
TEMPLATE                                                                 \
inline const ENUM_NAME&                                                  \
operator&=(ENUM_NAME& left, ENUM_NAME right)                             \
{ return left = left & right; }                                          \
                                                                         \
TEMPLATE                                                                 \
inline const ENUM_NAME&                                                  \
operator^=(ENUM_NAME& left, ENUM_NAME right)                             \
{ return left = left ^ right; }                                          \

