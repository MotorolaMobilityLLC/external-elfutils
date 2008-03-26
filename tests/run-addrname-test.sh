#! /bin/sh
# Copyright (C) 2007, 2008 Red Hat, Inc.
# This file is part of Red Hat elfutils.
#
# Red Hat elfutils is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# Red Hat elfutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Red Hat elfutils; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA.
#
# Red Hat elfutils is an included package of the Open Invention Network.
# An included package of the Open Invention Network is a package for which
# Open Invention Network licensees cross-license their patents.  No patent
# license is granted, either expressly or impliedly, by designation as an
# included package.  Should you wish to participate in the Open Invention
# Network licensing program, please visit www.openinventionnetwork.com
# <http://www.openinventionnetwork.com>.

. $srcdir/test-subr.sh

testfiles testfile34 testfile38 testfile41

testrun_compare ../src/addr2line -f -e testfile34 \
				 0x08048074 0x08048075 0x08048076 \
				 0x08049078 0x08048080 0x08049080 <<\EOF
foo
??:0
bar
??:0
_etext
??:0
data1
??:0
??
??:0
_end
??:0
EOF

testrun_compare ../src/addr2line -S -e testfile38 0x02 0x10a 0x211 0x31a <<\EOF
t1_global_outer+0x2
??:0
t2_global_symbol+0x2
??:0
t3_global_after_0+0x1
??:0
(.text)+0x31a
??:0
EOF

testrun_compare ../src/addr2line -S -e testfile41 0x1 0x104 <<\EOF
small_global_at_large_global+0x1
??:0
small_global_first_at_large_global+0x1
??:0
EOF

testfiles testfile12 testfile14
tempfiles testmaps

cat > testmaps <<EOF
00400000-00401000 r-xp 00000000 fd:01 4006812                            `pwd`/testfile14
00500000-00501000 rw-p 00000000 fd:01 4006812                            `pwd`/testfile14
01000000-01001000 r-xp 00000000 fd:01 1234567				 `pwd`/testfile12
01100000-01011000 rw-p 00000000 fd:01 1234567				 `pwd`/testfile12
2aaaaaaab000-2aaaaaaad000 rw-p 2aaaaaaab000 00:00 0 
2aaaaaae2000-2aaaaaae3000 rw-p 2aaaaaae2000 00:00 0 
7fff61068000-7fff6107d000 rw-p 7ffffffea000 00:00 0                      [stack]
7fff611fe000-7fff61200000 r-xp 7fff611fe000 00:00 0                      [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
EOF

testrun_compare ../src/addr2line -S -M testmaps 0x40047c 0x10009db <<\EOF
caller+0x14
/home/drepper/local/elfutils-build/20050425/v.c:11
foo+0xb
/home/drepper/local/elfutils-build/20030710/u.c:5
EOF

exit 0
