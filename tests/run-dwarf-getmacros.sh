#! /bin/sh
# Copyright (C) 2009 Red Hat, Inc.
# This file is part of elfutils.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# elfutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

. $srcdir/test-subr.sh

testfiles testfile51

testrun_compare ${abs_builddir}/dwarf-getmacros testfile51 0xb <<\EOF
__STDC__ 1
__STDC_HOSTED__ 1
__GNUC__ 4
__GNUC_MINOR__ 3
__GNUC_PATCHLEVEL__ 2
__GNUC_RH_RELEASE__ 7
__SIZE_TYPE__ long unsigned int
__PTRDIFF_TYPE__ long int
__WCHAR_TYPE__ int
__WINT_TYPE__ unsigned int
__INTMAX_TYPE__ long int
__UINTMAX_TYPE__ long unsigned int
__GXX_ABI_VERSION 1002
__SCHAR_MAX__ 127
__SHRT_MAX__ 32767
__INT_MAX__ 2147483647
__LONG_MAX__ 9223372036854775807L
__LONG_LONG_MAX__ 9223372036854775807LL
__WCHAR_MAX__ 2147483647
__CHAR_BIT__ 8
__INTMAX_MAX__ 9223372036854775807L
__FLT_EVAL_METHOD__ 0
__DEC_EVAL_METHOD__ 2
__FLT_RADIX__ 2
__FLT_MANT_DIG__ 24
__FLT_DIG__ 6
__FLT_MIN_EXP__ (-125)
__FLT_MIN_10_EXP__ (-37)
__FLT_MAX_EXP__ 128
__FLT_MAX_10_EXP__ 38
__FLT_MAX__ 3.40282347e+38F
__FLT_MIN__ 1.17549435e-38F
__FLT_EPSILON__ 1.19209290e-7F
__FLT_DENORM_MIN__ 1.40129846e-45F
__FLT_HAS_DENORM__ 1
__FLT_HAS_INFINITY__ 1
__FLT_HAS_QUIET_NAN__ 1
__DBL_MANT_DIG__ 53
__DBL_DIG__ 15
__DBL_MIN_EXP__ (-1021)
__DBL_MIN_10_EXP__ (-307)
__DBL_MAX_EXP__ 1024
__DBL_MAX_10_EXP__ 308
__DBL_MAX__ 1.7976931348623157e+308
__DBL_MIN__ 2.2250738585072014e-308
__DBL_EPSILON__ 2.2204460492503131e-16
__DBL_DENORM_MIN__ 4.9406564584124654e-324
__DBL_HAS_DENORM__ 1
__DBL_HAS_INFINITY__ 1
__DBL_HAS_QUIET_NAN__ 1
__LDBL_MANT_DIG__ 64
__LDBL_DIG__ 18
__LDBL_MIN_EXP__ (-16381)
__LDBL_MIN_10_EXP__ (-4931)
__LDBL_MAX_EXP__ 16384
__LDBL_MAX_10_EXP__ 4932
__DECIMAL_DIG__ 21
__LDBL_MAX__ 1.18973149535723176502e+4932L
__LDBL_MIN__ 3.36210314311209350626e-4932L
__LDBL_EPSILON__ 1.08420217248550443401e-19L
__LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
__LDBL_HAS_DENORM__ 1
__LDBL_HAS_INFINITY__ 1
__LDBL_HAS_QUIET_NAN__ 1
__DEC32_MANT_DIG__ 7
__DEC32_MIN_EXP__ (-95)
__DEC32_MAX_EXP__ 96
__DEC32_MIN__ 1E-95DF
__DEC32_MAX__ 9.999999E96DF
__DEC32_EPSILON__ 1E-6DF
__DEC32_DEN__ 0.000001E-95DF
__DEC64_MANT_DIG__ 16
__DEC64_MIN_EXP__ (-383)
__DEC64_MAX_EXP__ 384
__DEC64_MIN__ 1E-383DD
__DEC64_MAX__ 9.999999999999999E384DD
__DEC64_EPSILON__ 1E-15DD
__DEC64_DEN__ 0.000000000000001E-383DD
__DEC128_MANT_DIG__ 34
__DEC128_MIN_EXP__ (-6143)
__DEC128_MAX_EXP__ 6144
__DEC128_MIN__ 1E-6143DL
__DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
__DEC128_EPSILON__ 1E-33DL
__DEC128_DEN__ 0.000000000000000000000000000000001E-6143DL
__REGISTER_PREFIX__ 
__USER_LABEL_PREFIX__ 
__VERSION__ "4.3.2 20081105 (Red Hat 4.3.2-7)"
__GNUC_GNU_INLINE__ 1
_LP64 1
__LP64__ 1
__NO_INLINE__ 1
__FINITE_MATH_ONLY__ 0
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1
__SIZEOF_INT__ 4
__SIZEOF_LONG__ 8
__SIZEOF_LONG_LONG__ 8
__SIZEOF_SHORT__ 2
__SIZEOF_FLOAT__ 4
__SIZEOF_DOUBLE__ 8
__SIZEOF_LONG_DOUBLE__ 16
__SIZEOF_SIZE_T__ 8
__SIZEOF_WCHAR_T__ 4
__SIZEOF_WINT_T__ 4
__SIZEOF_PTRDIFF_T__ 8
__SIZEOF_POINTER__ 8
__amd64 1
__amd64__ 1
__x86_64 1
__x86_64__ 1
__k8 1
__k8__ 1
__MMX__ 1
__SSE__ 1
__SSE2__ 1
__SSE_MATH__ 1
__SSE2_MATH__ 1
__gnu_linux__ 1
__linux 1
__linux__ 1
linux 1
__unix 1
__unix__ 1
unix 1
__ELF__ 1
__DECIMAL_BID_FORMAT__ 1
macro1 ble
EOF

testrun_compare ${abs_builddir}/dwarf-getmacros testfile51 0x84 <<\EOF
__STDC__ 1
__STDC_HOSTED__ 1
__GNUC__ 4
__GNUC_MINOR__ 3
__GNUC_PATCHLEVEL__ 2
__GNUC_RH_RELEASE__ 7
__SIZE_TYPE__ long unsigned int
__PTRDIFF_TYPE__ long int
__WCHAR_TYPE__ int
__WINT_TYPE__ unsigned int
__INTMAX_TYPE__ long int
__UINTMAX_TYPE__ long unsigned int
__GXX_ABI_VERSION 1002
__SCHAR_MAX__ 127
__SHRT_MAX__ 32767
__INT_MAX__ 2147483647
__LONG_MAX__ 9223372036854775807L
__LONG_LONG_MAX__ 9223372036854775807LL
__WCHAR_MAX__ 2147483647
__CHAR_BIT__ 8
__INTMAX_MAX__ 9223372036854775807L
__FLT_EVAL_METHOD__ 0
__DEC_EVAL_METHOD__ 2
__FLT_RADIX__ 2
__FLT_MANT_DIG__ 24
__FLT_DIG__ 6
__FLT_MIN_EXP__ (-125)
__FLT_MIN_10_EXP__ (-37)
__FLT_MAX_EXP__ 128
__FLT_MAX_10_EXP__ 38
__FLT_MAX__ 3.40282347e+38F
__FLT_MIN__ 1.17549435e-38F
__FLT_EPSILON__ 1.19209290e-7F
__FLT_DENORM_MIN__ 1.40129846e-45F
__FLT_HAS_DENORM__ 1
__FLT_HAS_INFINITY__ 1
__FLT_HAS_QUIET_NAN__ 1
__DBL_MANT_DIG__ 53
__DBL_DIG__ 15
__DBL_MIN_EXP__ (-1021)
__DBL_MIN_10_EXP__ (-307)
__DBL_MAX_EXP__ 1024
__DBL_MAX_10_EXP__ 308
__DBL_MAX__ 1.7976931348623157e+308
__DBL_MIN__ 2.2250738585072014e-308
__DBL_EPSILON__ 2.2204460492503131e-16
__DBL_DENORM_MIN__ 4.9406564584124654e-324
__DBL_HAS_DENORM__ 1
__DBL_HAS_INFINITY__ 1
__DBL_HAS_QUIET_NAN__ 1
__LDBL_MANT_DIG__ 64
__LDBL_DIG__ 18
__LDBL_MIN_EXP__ (-16381)
__LDBL_MIN_10_EXP__ (-4931)
__LDBL_MAX_EXP__ 16384
__LDBL_MAX_10_EXP__ 4932
__DECIMAL_DIG__ 21
__LDBL_MAX__ 1.18973149535723176502e+4932L
__LDBL_MIN__ 3.36210314311209350626e-4932L
__LDBL_EPSILON__ 1.08420217248550443401e-19L
__LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
__LDBL_HAS_DENORM__ 1
__LDBL_HAS_INFINITY__ 1
__LDBL_HAS_QUIET_NAN__ 1
__DEC32_MANT_DIG__ 7
__DEC32_MIN_EXP__ (-95)
__DEC32_MAX_EXP__ 96
__DEC32_MIN__ 1E-95DF
__DEC32_MAX__ 9.999999E96DF
__DEC32_EPSILON__ 1E-6DF
__DEC32_DEN__ 0.000001E-95DF
__DEC64_MANT_DIG__ 16
__DEC64_MIN_EXP__ (-383)
__DEC64_MAX_EXP__ 384
__DEC64_MIN__ 1E-383DD
__DEC64_MAX__ 9.999999999999999E384DD
__DEC64_EPSILON__ 1E-15DD
__DEC64_DEN__ 0.000000000000001E-383DD
__DEC128_MANT_DIG__ 34
__DEC128_MIN_EXP__ (-6143)
__DEC128_MAX_EXP__ 6144
__DEC128_MIN__ 1E-6143DL
__DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
__DEC128_EPSILON__ 1E-33DL
__DEC128_DEN__ 0.000000000000000000000000000000001E-6143DL
__REGISTER_PREFIX__ 
__USER_LABEL_PREFIX__ 
__VERSION__ "4.3.2 20081105 (Red Hat 4.3.2-7)"
__GNUC_GNU_INLINE__ 1
_LP64 1
__LP64__ 1
__NO_INLINE__ 1
__FINITE_MATH_ONLY__ 0
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1
__SIZEOF_INT__ 4
__SIZEOF_LONG__ 8
__SIZEOF_LONG_LONG__ 8
__SIZEOF_SHORT__ 2
__SIZEOF_FLOAT__ 4
__SIZEOF_DOUBLE__ 8
__SIZEOF_LONG_DOUBLE__ 16
__SIZEOF_SIZE_T__ 8
__SIZEOF_WCHAR_T__ 4
__SIZEOF_WINT_T__ 4
__SIZEOF_PTRDIFF_T__ 8
__SIZEOF_POINTER__ 8
__amd64 1
__amd64__ 1
__x86_64 1
__x86_64__ 1
__k8 1
__k8__ 1
__MMX__ 1
__SSE__ 1
__SSE2__ 1
__SSE_MATH__ 1
__SSE2_MATH__ 1
__gnu_linux__ 1
__linux 1
__linux__ 1
linux 1
__unix 1
__unix__ 1
unix 1
__ELF__ 1
__DECIMAL_BID_FORMAT__ 1
macro2 ble
EOF

exit 0
