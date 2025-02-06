#pragma once 

#include <base/system/CompilerDetection.h>
#include <base/system/SystemDetection.h>


#if defined(OS_WIN) && defined(CPP_MSVC)
    #include <sal.h>
#endif


// Игнор SAL в случае компилятора, отличного от MSVC
#if defined(OS_WIN) && defined(CPP_MSVC)
    #ifndef _SAL2_In_reads_bytes_
        #define _SAL2_In_reads_bytes_		        _In_reads_bytes_
    #endif

    #ifndef _SAL2_Out_writes_bytes_
        #define _SAL2_Out_writes_bytes_		        _Out_writes_bytes_
    #endif

    #ifndef _SAL2_In_NLS_string_
        #define _SAL2_In_NLS_string_                _In_NLS_string_
    #endif

    #ifndef _SAL2_Out_writes_to_opt_ 
        #define _SAL2_Out_writes_to_opt_            _Out_writes_to_opt_
    #endif

    #ifndef _SAL2_Out_writes_bytes_to_opt_
        #define _SAL2_Out_writes_bytes_to_opt_      _Out_writes_bytes_to_opt_
    #endif

    #ifndef _SAL2_In
        #define _SAL2_In_					        _In_
    #endif

    #ifndef _SAL2_In_opt_
        #define _SAL2_In_opt_                       _In_opt_
    #endif

    #ifndef _SAL2_Out
        #define _SAL2_Out_					        _Out_
    #endif
    
    #ifndef _SAL2_Out_opt_
        #define _SAL2_Out_opt_                      _Out_opt_
    #endif      

    #ifndef _SAL2_In_z
        #define _SAL2_In_z_					        _In_z_
    #endif
#else
    #define _SAL2_In_reads_bytes_(size)		
    #define _SAL2_Out_writes_bytes_(size)	

    #define _SAL2_In_NLS_string_(size)
    #define _SAL2_Out_writes_to_opt_(size, count)

    #define _SAL2_Out_writes_bytes_to_opt_(size, count)

    #define _SAL2_In_						
    #define _SAL2_Out_		

    #define _SAL2_Out_opt_
    #define _SAL2_In_opt_
    #define _SAL2_In_z_						
#endif
