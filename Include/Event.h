//-----------------------------------------------------------------------------
//! @file Event.h
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

#ifdef In_EventH
#error "Inclusion circulaire de Event"
#endif // In_EventH

#define In_EventH

#ifndef EventH
#define EventH


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//                                  Evènements
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement générique.
//!
//! Ce type d'évènement est le type générique des évènements n'ayant pas
//! d'autre argument que @b Sender.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TNotifyEvent)(TObject *Sender);


#ifndef DOXYGEN

#ifdef UNICODE
#define TYPE_STD_STRING std::wstring
#define STRING_NULL L""
#else
#define TYPE_STD_STRING std::string
#define STRING_NULL ""
#endif  // UNICODE

class TEvent {
public:
  TObject *ObjCall;
};

template<typename Type, typename TypeArg1>
class TEvent1: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent1(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
			if (Call) (ObjCall->*Call)(Arg1);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

template<typename Type, typename TypeArg1, typename TypeArg2>
class TEvent2: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent2(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1, TypeArg2 Arg2) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
	    if (Call) (ObjCall->*Call)(Arg1, Arg2);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

template<typename Type, typename TypeArg1, typename TypeArg2, typename TypeArg3>
class TEvent3: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent3(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1, TypeArg2 Arg2, TypeArg3 Arg3) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
	    if (Call) (ObjCall->*Call)(Arg1, Arg2, Arg3);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

template<typename Type, typename TypeArg1, typename TypeArg2, typename TypeArg3, typename TypeArg4>
class TEvent4: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent4(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1, TypeArg2 Arg2, TypeArg3 Arg3, TypeArg4 Arg4) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
	    if (Call) (ObjCall->*Call)(Arg1, Arg2, Arg3, Arg4);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

template<typename Type, typename TypeArg1, typename TypeArg2, typename TypeArg3, typename TypeArg4, typename TypeArg5>
class TEvent5: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent5(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1, TypeArg2 Arg2, TypeArg3 Arg3, TypeArg4 Arg4, TypeArg5 Arg5) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
	    if (Call) (ObjCall->*Call)(Arg1, Arg2, Arg3, Arg4, Arg5);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

template<typename Type, typename TypeArg1, typename TypeArg2, typename TypeArg3, typename TypeArg4, typename TypeArg5, typename TypeArg6>
class TEvent6: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent6(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1, TypeArg2 Arg2, TypeArg3 Arg3, TypeArg4 Arg4, TypeArg5 Arg5, TypeArg6 Arg6) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
	    if (Call) (ObjCall->*Call)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

template<typename Type, typename TypeArg1, typename TypeArg2, typename TypeArg3, typename TypeArg4, typename TypeArg5, typename TypeArg6, typename TypeArg7>
class TEvent7: public TEvent {
private:
	TYPE_STD_STRING sEventCall;
  Type Call;
public:

  TEvent7(void) {
    ObjCall = NULL;
    Call = NULL;
  }

  inline void operator=(int EventCall) {
    if (EventCall == 0) Call = NULL;
  }

  inline void operator=(Type EventCall) {
    Call = EventCall;
  }

  inline void operator=(const TCHAR *EventCall) {
		sEventCall = EventCall;
  }

  inline void operator()(TypeArg1 Arg1, TypeArg2 Arg2, TypeArg3 Arg3, TypeArg4 Arg4, TypeArg5 Arg5, TypeArg6 Arg6, TypeArg7 Arg7) {
		if (ObjCall) {
			if (Call == NULL && sEventCall != STRING_NULL) {
				ObjCall->SetEvent(this, sEventCall.c_str());
			}
	    if (Call) (ObjCall->*Call)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
		}
  }

  inline operator bool() {
    return (ObjCall != NULL && (Call != NULL || sEventCall != STRING_NULL));
  }

  inline const TCHAR *AsString() {
    if (ObjCall) {
			if (Call != NULL && sEventCall == STRING_NULL) {
				sEventCall = ObjCall->GetEvent(this);
			}
		}
		return sEventCall.c_str();
  }

};

#define DECLARE_EVENT_1(EventType,EventName,type1) TEvent1<EventType, type1> EventName
#define DECLARE_EVENT_2(EventType,EventName,type1,type2) TEvent2<EventType, type1, type2> EventName
#define DECLARE_EVENT_3(EventType,EventName,type1,type2,type3) TEvent3<EventType, type1, type2, type3> EventName
#define DECLARE_EVENT_4(EventType,EventName,type1,type2,type3,type4) TEvent4<EventType, type1, type2, type3, type4> EventName
#define DECLARE_EVENT_5(EventType,EventName,type1,type2,type3,type4,type5) TEvent5<EventType, type1, type2, type3, type4, type5> EventName
#define DECLARE_EVENT_6(EventType,EventName,type1,type2,type3,type4,type5,type6) TEvent6<EventType, type1, type2, type3, type4, type5, type6> EventName
#define DECLARE_EVENT_7(EventType,EventName,type1,type2,type3,type4,type5,type6,type7) TEvent7<EventType, type1, type2, type3, type4, type5, type6, type7> EventName

//-----------------------------------------------------------------------------
// Set event = fonction
// ----------------------------------------------------------------------------

#define SET_EVENT(objevent, EventType, EventName, classcall, objcall, fnctcall)\
{                                                                              \
  objevent->EventName.ObjCall = (objcall);                                     \
	objevent->EventName = (EventType) &classcall::fnctcall;                      \
}

//-----------------------------------------------------------------------------
// Set event = string pour la fonction SetEvent
// ----------------------------------------------------------------------------

#ifdef UNICODE

#define SET_EVENT_STR_1(Class, EventType, EventName, FnctName, Type1)          \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent1<EventType, Type1> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_2(Class, EventType, EventName, FnctName, Type1, Type2)   \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent2<EventType, Type1, Type2> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_3(Class, EventType, EventName, FnctName, Type1, Type2, Type3) \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent3<EventType, Type1, Type2, Type3> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_4(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4) \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent4<EventType, Type1, Type2, Type3, Type4> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_5(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4, Type5) \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent5<EventType, Type1, Type2, Type3, Type4, Type5> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_6(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4, Type5, Type6) \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent6<EventType, Type1, Type2, Type3, Type4, Type5, Type6> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_7(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4, Type5, Type6, Type7) \
  if (!lstrcmp(EventCall, L#FnctName))                                         \
  reinterpret_cast<TEvent7<EventType, Type1, Type2, Type3, Type4, Type5, Type6, Type7> *>(Event)->operator=((EventType) &Class::FnctName);

#else  // UNICODE

#define SET_EVENT_STR_1(Class, EventType, EventName, FnctName, Type1)          \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent1<EventType, Type1> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_2(Class, EventType, EventName, FnctName, Type1, Type2)   \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent2<EventType, Type1, Type2> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_3(Class, EventType, EventName, FnctName, Type1, Type2, Type3) \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent3<EventType, Type1, Type2, Type3> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_4(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4) \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent4<EventType, Type1, Type2, Type3, Type4> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_5(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4, Type5) \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent5<EventType, Type1, Type2, Type3, Type4, Type5> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_6(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4, Type5, Type6) \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent6<EventType, Type1, Type2, Type3, Type4, Type5, Type6> *>(Event)->operator=((EventType) &Class::FnctName);

#define SET_EVENT_STR_7(Class, EventType, EventName, FnctName, Type1, Type2, Type3, Type4, Type5, Type6, Type7) \
  if (!lstrcmp(EventCall, #FnctName))                                         \
  reinterpret_cast<TEvent7<EventType, Type1, Type2, Type3, Type4, Type5, Type6, Type7> *>(Event)->operator=((EventType) &Class::FnctName);

#endif  // UNICODE


#else // DOXYGEN

#define DECLARE_EVENT_1(EventType,EventName,type1) __property EventType EventName(type1)
#define DECLARE_EVENT_2(EventType,EventName,type1,type2) __property EventType EventName(type1,type2)
#define DECLARE_EVENT_3(EventType,EventName,type1,type2,type3) __property EventType EventName(type1,type2,type3)
#define DECLARE_EVENT_4(EventType,EventName,type1,type2,type3,type4) __property EventType EventName(type1,type2,type3,type4)
#define DECLARE_EVENT_5(EventType,EventName,type1,type2,type3,type4,type5) __property EventType EventName(type1,type2,type3,type4,type5)
#define DECLARE_EVENT_6(EventType,EventName,type1,type2,type3,type4,type5,type6) __property EventType EventName(type1,type2,type3,type4,type5,type6)
#define DECLARE_EVENT_7(EventType,EventName,type1,type2,type3,type4,type5,type6,type7) __property EventType EventName(type1,type2,type3,type4,type5,type6,type7)

#endif // DOXYGEN

#endif  // EventH

#undef In_EventH
