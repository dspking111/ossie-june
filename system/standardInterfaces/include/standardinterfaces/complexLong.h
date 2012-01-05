// This file is generated by omniidl (C++ backend)- omniORB_4_1. Do not edit.
#ifndef __complexLong_hh__
#define __complexLong_hh__

#ifndef __CORBA_H_EXTERNAL_GUARD__
#include <omniORB4/CORBA.h>
#endif

#ifndef  USE_stub_in_nt_dll
# define USE_stub_in_nt_dll_NOT_DEFINED_complexLong
#endif
#ifndef  USE_core_stub_in_nt_dll
# define USE_core_stub_in_nt_dll_NOT_DEFINED_complexLong
#endif
#ifndef  USE_dyn_stub_in_nt_dll
# define USE_dyn_stub_in_nt_dll_NOT_DEFINED_complexLong
#endif



#ifndef __PortTypes_hh_EXTERNAL_GUARD__
#define __PortTypes_hh_EXTERNAL_GUARD__
#include "ossie/PortTypes.h"
#endif



#ifdef USE_stub_in_nt_dll
# ifndef USE_core_stub_in_nt_dll
#  define USE_core_stub_in_nt_dll
# endif
# ifndef USE_dyn_stub_in_nt_dll
#  define USE_dyn_stub_in_nt_dll
# endif
#endif

#ifdef _core_attr
# error "A local CPP macro _core_attr has already been defined."
#else
# ifdef  USE_core_stub_in_nt_dll
#  define _core_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _core_attr
# endif
#endif

#ifdef _dyn_attr
# error "A local CPP macro _dyn_attr has already been defined."
#else
# ifdef  USE_dyn_stub_in_nt_dll
#  define _dyn_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _dyn_attr
# endif
#endif





_CORBA_MODULE standardInterfaces

_CORBA_MODULE_BEG

#ifndef __standardInterfaces_mcomplexLong__
#define __standardInterfaces_mcomplexLong__

  class complexLong;
  class _objref_complexLong;
  class _impl_complexLong;
  
  typedef _objref_complexLong* complexLong_ptr;
  typedef complexLong_ptr complexLongRef;

  class complexLong_Helper {
  public:
    typedef complexLong_ptr _ptr_type;

    static _ptr_type _nil();
    static _CORBA_Boolean is_nil(_ptr_type);
    static void release(_ptr_type);
    static void duplicate(_ptr_type);
    static void marshalObjRef(_ptr_type, cdrStream&);
    static _ptr_type unmarshalObjRef(cdrStream&);
  };

  typedef _CORBA_ObjRef_Var<_objref_complexLong, complexLong_Helper> complexLong_var;
  typedef _CORBA_ObjRef_OUT_arg<_objref_complexLong,complexLong_Helper > complexLong_out;

#endif

  // interface complexLong
  class complexLong {
  public:
    // Declarations for this interface type.
    typedef complexLong_ptr _ptr_type;
    typedef complexLong_var _var_type;

    static _ptr_type _duplicate(_ptr_type);
    static _ptr_type _narrow(::CORBA::Object_ptr);
    static _ptr_type _unchecked_narrow(::CORBA::Object_ptr);
    
    static _ptr_type _nil();

    static inline void _marshalObjRef(_ptr_type, cdrStream&);

    static inline _ptr_type _unmarshalObjRef(cdrStream& s) {
      omniObjRef* o = omniObjRef::_unMarshal(_PD_repoId,s);
      if (o)
        return (_ptr_type) o->_ptrToObjRef(_PD_repoId);
      else
        return _nil();
    }

    static _core_attr const char* _PD_repoId;

    // Other IDL defined within this scope.
    
  };

  class _objref_complexLong :
    public virtual ::CORBA::Object,
    public virtual omniObjRef
  {
  public:
    void pushPacket(const PortTypes::LongSequence& I, const PortTypes::LongSequence& Q);

    inline _objref_complexLong()  { _PR_setobj(0); }  // nil
    _objref_complexLong(omniIOR*, omniIdentity*);

  protected:
    virtual ~_objref_complexLong();

    
  private:
    virtual void* _ptrToObjRef(const char*);

    _objref_complexLong(const _objref_complexLong&);
    _objref_complexLong& operator = (const _objref_complexLong&);
    // not implemented

    friend class complexLong;
  };

  class _pof_complexLong : public _OMNI_NS(proxyObjectFactory) {
  public:
    inline _pof_complexLong() : _OMNI_NS(proxyObjectFactory)(complexLong::_PD_repoId) {}
    virtual ~_pof_complexLong();

    virtual omniObjRef* newObjRef(omniIOR*,omniIdentity*);
    virtual _CORBA_Boolean is_a(const char*) const;
  };

  class _impl_complexLong :
    public virtual omniServant
  {
  public:
    virtual ~_impl_complexLong();

    virtual void pushPacket(const PortTypes::LongSequence& I, const PortTypes::LongSequence& Q) = 0;
    
  public:  // Really protected, workaround for xlC
    virtual _CORBA_Boolean _dispatch(omniCallHandle&);

  private:
    virtual void* _ptrToInterface(const char*);
    virtual const char* _mostDerivedRepoId();
    
  };


  _CORBA_MODULE_VAR _dyn_attr const ::CORBA::TypeCode_ptr _tc_complexLong;

_CORBA_MODULE_END



_CORBA_MODULE POA_standardInterfaces
_CORBA_MODULE_BEG

  class complexLong :
    public virtual standardInterfaces::_impl_complexLong,
    public virtual ::PortableServer::ServantBase
  {
  public:
    virtual ~complexLong();

    inline ::standardInterfaces::complexLong_ptr _this() {
      return (::standardInterfaces::complexLong_ptr) _do_this(::standardInterfaces::complexLong::_PD_repoId);
    }
  };

_CORBA_MODULE_END



_CORBA_MODULE OBV_standardInterfaces
_CORBA_MODULE_BEG

_CORBA_MODULE_END





#undef _core_attr
#undef _dyn_attr

void operator<<=(::CORBA::Any& _a, standardInterfaces::complexLong_ptr _s);
void operator<<=(::CORBA::Any& _a, standardInterfaces::complexLong_ptr* _s);
_CORBA_Boolean operator>>=(const ::CORBA::Any& _a, standardInterfaces::complexLong_ptr& _s);



inline void
standardInterfaces::complexLong::_marshalObjRef(::standardInterfaces::complexLong_ptr obj, cdrStream& s) {
  omniObjRef::_marshal(obj->_PR_getobj(),s);
}




#ifdef   USE_stub_in_nt_dll_NOT_DEFINED_complexLong
# undef  USE_stub_in_nt_dll
# undef  USE_stub_in_nt_dll_NOT_DEFINED_complexLong
#endif
#ifdef   USE_core_stub_in_nt_dll_NOT_DEFINED_complexLong
# undef  USE_core_stub_in_nt_dll
# undef  USE_core_stub_in_nt_dll_NOT_DEFINED_complexLong
#endif
#ifdef   USE_dyn_stub_in_nt_dll_NOT_DEFINED_complexLong
# undef  USE_dyn_stub_in_nt_dll
# undef  USE_dyn_stub_in_nt_dll_NOT_DEFINED_complexLong
#endif

#endif  // __complexLong_hh__

