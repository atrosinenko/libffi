/* -----------------------------------------------------------------------
   ffi.c - Copyright (c) 2015 Anatoly Trosinenko 

   Emscripten Foreign Function Interface

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

#include <ffi.h>
#include <ffi_common.h>
#include <stdlib.h>
#include <emscripten.h>
#include <stdio.h>

/* Perform machine dependent cif processing.  */
ffi_status FFI_HIDDEN
ffi_prep_cif_machdep(ffi_cif *cif)
{
	if(cif->rtype->size != 1 &&
		cif->rtype->size != 2 &&
		cif->rtype->size != 4 &&
		cif->rtype->type != FFI_TYPE_DOUBLE &&
		cif->rtype->type != FFI_TYPE_LONGDOUBLE)
	{
		return FFI_BAD_TYPEDEF;
	}
	return FFI_OK;
}

static const char *type_character(const ffi_type *type)
{
	switch(type->type)
	{
	case FFI_TYPE_FLOAT:
	case FFI_TYPE_DOUBLE:
#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
	case FFI_TYPE_LONGDOUBLE:
#endif
		return "d";
	default:
		return "i";
	}
}

static int type_logsize(const ffi_type *type)
{
	switch(type->size)
	{
	case 1: return 0;
	case 2: return 1;
	case 4: return 2;
	case 8: return 3;
	}
}

#define PUT_JS(str) ptr = stpcpy(ptr, (str))
#define PRINTF_JS(...) ptr += sprintf(ptr, __VA_ARGS__)

void
ffi_call (ffi_cif *cif, void (*fn)(void), void *rvalue, void **avalue)
{
	char *buf = malloc(25 * cif->nargs + 50);
	buf[0] = 0;
	char *ptr = buf;
	int i;
	switch(cif->rtype->type)
	{
	case FFI_TYPE_FLOAT:
		PRINTF_JS("HEAPF32[%d>>2] = ", rvalue);
		break;
	case FFI_TYPE_DOUBLE:
#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
	case FFI_TYPE_LONGDOUBLE:
#endif
		PRINTF_JS("HEAPF64[%d>>3] = ", rvalue);
		break;
	default:
		PRINTF_JS("HEAP32[%d>>2] = ", rvalue);
		break;
	}
	PUT_JS("dynCall_");
	PUT_JS(type_character(cif->rtype));
	for(i = 0; i < cif->nargs; ++i)
	{
		PUT_JS(type_character(cif->arg_types[i]));
	}
	PRINTF_JS("(%d,", fn);
	for(i = 0; i < cif->nargs; ++i)
	{
		if(i > 0)
		{
			PUT_JS(",");
		}
		switch(cif->arg_types[i]->type)
		{
		case FFI_TYPE_FLOAT:
			PRINTF_JS("HEAPF32[%d>>2]", avalue[i]);
			break;
		case FFI_TYPE_DOUBLE:
#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
		case FFI_TYPE_LONGDOUBLE:
#endif
			PRINTF_JS("HEAPF64[%d>>3]", avalue[i]);
			break;
		default:
			PRINTF_JS("HEAP%d[%d>>%d]", cif->arg_types[i]->size * 8, avalue[i], type_logsize(cif->arg_types[i]));
			break;
		}
	}
	PUT_JS(")");
	emscripten_run_script(buf);
	free(buf);
}

