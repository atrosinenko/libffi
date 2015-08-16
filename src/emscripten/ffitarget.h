/* -----------------------------------------------------------------*-C-*-
   ffitarget.h - Copyright (c) 2015  Anatoly Trosinenko

   Target configuration macros for Emscripten C-to-JS compiler.

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   ``Software''), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   ----------------------------------------------------------------------- */

#ifndef LIBFFI_TARGET_H
#define LIBFFI_TARGET_H

#ifndef LIBFFI_H
#error "Please do not include ffitarget.h directly into your source.  Use ffi.h instead."
#endif

/* ---- System specific configurations ----------------------------------- */

//#define USE_BUILTIN_FFS 0 

/* ---- Generic type definitions ----------------------------------------- */

#if defined __x86_64__ && defined __ILP32__
#define FFI_SIZEOF_ARG 8
#define FFI_SIZEOF_JAVA_RAW  4
typedef unsigned long long     ffi_arg;
typedef long long              ffi_sarg;
#else
typedef unsigned long          ffi_arg;
typedef signed long            ffi_sarg;
#endif

typedef enum ffi_abi {
  FFI_FIRST_ABI = 0,
  FFI_EMSCRIPTEN = 1,
  FFI_LAST_ABI,
  FFI_DEFAULT_ABI = FFI_EMSCRIPTEN
} ffi_abi;

#define FFI_NATIVE_RAW_API 0
#define FFI_NO_RAW_API 1

#endif

