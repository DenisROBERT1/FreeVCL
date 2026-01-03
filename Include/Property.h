//-----------------------------------------------------------------------------
//! @file Property.h
//!
//! @author Denis ROBERT (http://denis.beru.free.fr)
//!
//! Copyright 2008-2025
//! @n
//! This file is part of FreeVCL.
//! @n
//!     FreeVCL is free software: you can redistribute it and/or modify
//!     it under the terms of the GNU Lesser General Public License as
//!     published by the Free Software Foundation, either version 3 of
//!     the License, or (at your option) any later version.
//! @n
//!     FreeVCL is distributed in the hope that it will be useful,
//!     but WITHOUT ANY WARRANTY; without even the implied warranty of
//!     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//!     GNU Lesser General Public License for more details.
//! @n
//!     You should have received a copy of the GNU Lesser General Public
//!     License along with FreeVCL.
//!     If not, see <http://www.gnu.org/licenses/>.
//!
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_PropertyH
#error "Inclusion circulaire de Property"
#endif // In_PropertyH

#define In_PropertyH

#ifndef PropertyH
#define PropertyH


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//                                  Propriétés
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//                      __BORLANDC__ (C++ Builder)
// -----------------------------------------------------------------------------

#ifdef __BORLANDC__

#define DECLARE_PROPERTY_GET(base, type, var)                                  \
protected:                                                                     \
virtual type Get_##var(void);                                                  \
public:                                                                        \
__property type var = {read = Get_##var}

#define DECLARE_PROPERTY_TAB_GET(base, type, var)                              \
protected:                                                                     \
virtual type Get_##var(int);                                                   \
public:                                                                        \
__property type var[int i] = {read = Get_##var}

#define DECLARE_PROPERTY(base, type, var)                                      \
protected:                                                                     \
virtual type Get_##var(void);                                                  \
virtual bool Set_##var(type);                                                  \
public:                                                                        \
__property type var = {read = Get_##var, write = Set_##var}

#define DECLARE_PROPERTY_TAB(base, type, var)                                  \
protected:                                                                     \
virtual type Get_##var(int);                                                   \
public:                                                                        \
virtual bool Set_##var(int, type);                                             \
__property type var[int i] = {read = Get_##var, write = Set_##var}

#endif  // __BORLANDC__

// -----------------------------------------------------------------------------
//                            MSC (VISUAL STUDIO)
// -----------------------------------------------------------------------------

#ifdef _MSC_VER

#define DECLARE_PROPERTY_GET(base, type, var)                                  \
public:                                                                        \
virtual type Get_##var(void);                                                  \
__declspec(property(get = Get_##var)) type var

#define DECLARE_PROPERTY_TAB_GET(base, type, var)                              \
public:                                                                        \
virtual type Get_##var(int);                                                   \
__declspec(property(get = Get_##var)) type var[]

#define DECLARE_PROPERTY(base, type, var)                                      \
public:                                                                        \
virtual type Get_##var(void);                                                  \
virtual bool Set_##var(type);                                                  \
__declspec(property(get = Get_##var, put = Set_##var)) type var

#define DECLARE_PROPERTY_TAB(base, type, var)                                  \
public:                                                                        \
virtual type Get_##var(int);                                                   \
virtual bool Set_##var(int, type);                                             \
__declspec(property(get = Get_##var, put = Set_##var)) type var[]

#endif  // _MSC_VER

// -----------------------------------------------------------------------------
//                                 DOXYGEN
// -----------------------------------------------------------------------------


#ifdef DOXYGEN
#define DECLARE_PROPERTY_GET(base, type, var) const type var
#define DECLARE_PROPERTY_TAB_GET(base, type, var) const type var[]
#define DECLARE_PROPERTY(base, type, var) type var
#define DECLARE_PROPERTY_TAB(base, type, var) type var[]
#endif  // DOXYGEN

// -----------------------------------------------------------------------------
//                     __GNUG__ (Code-blocks, DEV-CPP)
// -----------------------------------------------------------------------------

#ifdef __GNUG__

#define DECLARE_PROPERTY_GET( base, type, var )                                \
protected:                                                                     \
virtual type Get_##var(void);                                                  \
public:                                                                        \
struct _dp_##Get##var                                                          \
{                                                                              \
    inline operator type()                                                     \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var();               \
    }                                                                          \
    inline bool operator==(type const & value)                                 \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var() == value;      \
    }                                                                          \
    inline bool operator!=(type const & value)                                 \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var() != value;      \
    }                                                                          \
    inline type operator->()                                                   \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var();               \
    }                                                                          \
};                                                                             \
friend struct _dp_##Get##var;                                                  \
_dp_##Get##var var

#define DECLARE_PROPERTY_TAB_GET( base, type, var )                            \
protected:                                                                     \
virtual type Get_##var(int);                                                   \
public:                                                                        \
struct _dp_##Get##var                                                          \
{                                                                              \
    inline type operator[] (int i)                                             \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var( i );            \
    }                                                                          \
};                                                                             \
friend struct _dp_##Get##var;                                                  \
_dp_##Get##var var

#define DECLARE_PROPERTY_PUT( base, type, var )                                \
protected:                                                                     \
virtual bool Set_##var(type);                                                  \
public:                                                                        \
struct _dp_##Set##var                                                          \
{                                                                              \
    inline type operator=(type const & value)                                  \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Set_##var( value );        \
    }                                                                          \
};                                                                             \
friend struct _dp_##Set##var;                                                  \
_dp_##Set##var var

#define DECLARE_PROPERTY( base, type, var )                                    \
protected:                                                                     \
virtual type Get_##var(void);                                                  \
virtual bool Set_##var(type);                                                  \
public:                                                                        \
struct _dp_##Get##var##_##Set##var                                             \
{                                                                              \
    inline operator type()                                                     \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var();               \
    }                                                                          \
    inline type operator=(type const & value)                                  \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        ((base *) ((char *) this - Offset))->Set_##var( value );               \
        return value ;                                                         \
    }                                                                          \
    inline bool operator==(type const & value)                                 \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var() == value;      \
    }                                                                          \
    inline bool operator!=(type const & value)                                 \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var() != value;      \
    }                                                                          \
    inline type operator->()                                                   \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var();               \
    }                                                                          \
};                                                                             \
friend struct _dp_##Get##var##_##Set##var;                                     \
_dp_##Get##var##_##Set##var var

#define DECLARE_PROPERTY_TAB( base, type, var )                                \
protected:                                                                     \
virtual type Get_##var(int);                                                   \
public:                                                                        \
virtual bool Set_##var(int, type);                                             \
struct _dp_##Get##var##_##Set##var                                             \
{                                                                              \
    int memo_i;                                                                \
    inline type operator[] (int i)                                             \
    {                                                                          \
        memo_i = i;                                                            \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        return ((base *) ((char *) this - Offset))->Get_##var( i );            \
    }                                                                          \
    inline type operator=(type const & value)                                  \
    {                                                                          \
        int Offset = (size_t) ((char *) &((base *) this)->var - (char *) this);\
        ((base *) ((char *) this - Offset))->Set_##var(memo_i, value );        \
        return value ;                                                         \
    }                                                                          \
};                                                                             \
friend struct _dp_##Get##var##_##Set##var;                                     \
_dp_##Get##var##_##Set##var var

#endif  // __GNUG__

#endif  // PropertyH

#undef In_PropertyH
