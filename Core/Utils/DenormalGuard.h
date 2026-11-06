#pragma once

#if defined( __SSE__ ) || defined( _M_X64 ) || ( defined( _M_IX86_FP ) && _M_IX86_FP > 0 )
#define CORE_HAS_SSE_DENORMAL_CONTROL 1
#include <pmmintrin.h>
#include <xmmintrin.h>
#endif

namespace Core {

class DenormalGuard {

public:
    DenormalGuard() {
#ifdef CORE_HAS_SSE_DENORMAL_CONTROL
        _previousFlush = _MM_GET_FLUSH_ZERO_MODE();
        _previousDenormals = _MM_GET_DENORMALS_ZERO_MODE();
        _MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
        _MM_SET_DENORMALS_ZERO_MODE( _MM_DENORMALS_ZERO_ON );
#endif
    }

    ~DenormalGuard() {
#ifdef CORE_HAS_SSE_DENORMAL_CONTROL
        _MM_SET_FLUSH_ZERO_MODE( _previousFlush );
        _MM_SET_DENORMALS_ZERO_MODE( _previousDenormals );
#endif
    }

    DenormalGuard( const DenormalGuard& ) = delete;
    DenormalGuard& operator=( const DenormalGuard& ) = delete;

private:
#ifdef CORE_HAS_SSE_DENORMAL_CONTROL
    unsigned int _previousFlush;
    unsigned int _previousDenormals;
#endif
};

} // namespace Core
