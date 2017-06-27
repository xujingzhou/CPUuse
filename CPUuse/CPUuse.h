

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Jun 27 14:19:59 2017
 */
/* Compiler settings for CPUuse.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CPUuse_h__
#define __CPUuse_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICpuUsage_FWD_DEFINED__
#define __ICpuUsage_FWD_DEFINED__
typedef interface ICpuUsage ICpuUsage;
#endif 	/* __ICpuUsage_FWD_DEFINED__ */


#ifndef __CpuUsage_FWD_DEFINED__
#define __CpuUsage_FWD_DEFINED__

#ifdef __cplusplus
typedef class CpuUsage CpuUsage;
#else
typedef struct CpuUsage CpuUsage;
#endif /* __cplusplus */

#endif 	/* __CpuUsage_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICpuUsage_INTERFACE_DEFINED__
#define __ICpuUsage_INTERFACE_DEFINED__

/* interface ICpuUsage */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICpuUsage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F6F0ADA0-32FA-11D7-AA35-EA02DB2FF40A")
    ICpuUsage : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsShellSystemType( 
            /* [retval][out] */ BSTR *pbstrSysType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CpuUsage9x( 
            /* [retval][out] */ long *plResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CpuUsage2000( 
            /* [retval][out] */ long *plResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICpuUsageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICpuUsage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICpuUsage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICpuUsage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICpuUsage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICpuUsage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICpuUsage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICpuUsage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsShellSystemType )( 
            ICpuUsage * This,
            /* [retval][out] */ BSTR *pbstrSysType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CpuUsage9x )( 
            ICpuUsage * This,
            /* [retval][out] */ long *plResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CpuUsage2000 )( 
            ICpuUsage * This,
            /* [retval][out] */ long *plResult);
        
        END_INTERFACE
    } ICpuUsageVtbl;

    interface ICpuUsage
    {
        CONST_VTBL struct ICpuUsageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICpuUsage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICpuUsage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICpuUsage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICpuUsage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICpuUsage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICpuUsage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICpuUsage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICpuUsage_IsShellSystemType(This,pbstrSysType)	\
    ( (This)->lpVtbl -> IsShellSystemType(This,pbstrSysType) ) 

#define ICpuUsage_CpuUsage9x(This,plResult)	\
    ( (This)->lpVtbl -> CpuUsage9x(This,plResult) ) 

#define ICpuUsage_CpuUsage2000(This,plResult)	\
    ( (This)->lpVtbl -> CpuUsage2000(This,plResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICpuUsage_INTERFACE_DEFINED__ */



#ifndef __CPUUSELib_LIBRARY_DEFINED__
#define __CPUUSELib_LIBRARY_DEFINED__

/* library CPUUSELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CPUUSELib;

EXTERN_C const CLSID CLSID_CpuUsage;

#ifdef __cplusplus

class DECLSPEC_UUID("F6F0ADA1-32FA-11D7-AA35-EA02DB2FF40A")
CpuUsage;
#endif
#endif /* __CPUUSELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


