#! /bin/sh
# Copyright (C) 2005, 2006 Red Hat, Inc.
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
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
#
# Red Hat elfutils is an included package of the Open Invention Network.
# An included package of the Open Invention Network is a package for which
# Open Invention Network licensees cross-license their patents.  No patent
# license is granted, either expressly or impliedly, by designation as an
# included package.  Should you wish to participate in the Open Invention
# Network licensing program, please visit www.openinventionnetwork.com
# <http://www.openinventionnetwork.com>.

. $srcdir/test-subr.sh

regs_test()
{
  tempfiles good.regs
  cat > good.regs
  testfiles "$@"
  for f; do testrun_compare ./allregs -e $f < good.regs; done
  test_cleanup
}

regs_test testfile11 <<\EOF
integer registers:
	  0: %eax (eax)
	  1: %ecx (ecx)
	  2: %edx (edx)
	  3: %ebx (ebx)
	  4: %esp (esp)
	  5: %ebp (ebp)
	  6: %esi (esi)
	  7: %edi (edi)
	  8: %eip (eip)
	  9: %eflags (eflags)
	 10: %trapno (trapno)
FPU-control registers:
	 37: %fctrl (fctrl)
	 38: %fstat (fstat)
	 39: %mxcsr (mxcsr)
MMX registers:
	 29: %mm0 (mm0)
	 30: %mm1 (mm1)
	 31: %mm2 (mm2)
	 32: %mm3 (mm3)
	 33: %mm4 (mm4)
	 34: %mm5 (mm5)
	 35: %mm6 (mm6)
	 36: %mm7 (mm7)
SSE registers:
	 21: %xmm0 (xmm0)
	 22: %xmm1 (xmm1)
	 23: %xmm2 (xmm2)
	 24: %xmm3 (xmm3)
	 25: %xmm4 (xmm4)
	 26: %xmm5 (xmm5)
	 27: %xmm6 (xmm6)
	 28: %xmm7 (xmm7)
segment registers:
	 40: %es (es)
	 41: %cs (cs)
	 42: %ss (ss)
	 43: %ds (ds)
	 44: %fs (fs)
	 45: %gs (gs)
x87 registers:
	 11: %st0 (st0)
	 12: %st1 (st1)
	 13: %st2 (st2)
	 14: %st3 (st3)
	 15: %st4 (st4)
	 16: %st5 (st5)
	 17: %st6 (st6)
	 18: %st7 (st7)
EOF

regs_test testfile12 <<\EOF
integer registers:
	  0: %rax (rax)
	  1: %rdx (rdx)
	  2: %rcx (rcx)
	  3: %rbx (rbx)
	  4: %rsi (rsi)
	  5: %rdi (rdi)
	  6: %rbp (rbp)
	  7: %rsp (rsp)
	  8: %r8 (r8)
	  9: %r9 (r9)
	 10: %r10 (r10)
	 11: %r11 (r11)
	 12: %r12 (r12)
	 13: %r13 (r13)
	 14: %r14 (r14)
	 15: %r15 (r15)
	 16: %rip (rip)
MMX registers:
	 41: %mm0 (mm0)
	 42: %mm1 (mm1)
	 43: %mm2 (mm2)
	 44: %mm3 (mm3)
	 45: %mm4 (mm4)
	 46: %mm5 (mm5)
	 47: %mm6 (mm6)
	 48: %mm7 (mm7)
SSE registers:
	 17: %xmm0 (xmm0)
	 18: %xmm1 (xmm1)
	 19: %xmm2 (xmm2)
	 20: %xmm3 (xmm3)
	 21: %xmm4 (xmm4)
	 22: %xmm5 (xmm5)
	 23: %xmm6 (xmm6)
	 24: %xmm7 (xmm7)
	 25: %xmm8 (xmm8)
	 26: %xmm9 (xmm9)
	 27: %xmm10 (xmm10)
	 28: %xmm11 (xmm11)
	 29: %xmm12 (xmm12)
	 30: %xmm13 (xmm13)
	 31: %xmm14 (xmm14)
	 32: %xmm15 (xmm15)
x87 registers:
	 33: %st0 (st0)
	 34: %st1 (st1)
	 35: %st2 (st2)
	 36: %st3 (st3)
	 37: %st4 (st4)
	 38: %st5 (st5)
	 39: %st6 (st6)
	 40: %st7 (st7)
EOF

regs_test testfile2 testfile23 <<\EOF
integer registers:
	  0: r0 (r0)
	  1: r1 (r1)
	  2: r2 (r2)
	  3: r3 (r3)
	  4: r4 (r4)
	  5: r5 (r5)
	  6: r6 (r6)
	  7: r7 (r7)
	  8: r8 (r8)
	  9: r9 (r9)
	 10: r10 (r10)
	 11: r11 (r11)
	 12: r12 (r12)
	 13: r13 (r13)
	 14: r14 (r14)
	 15: r15 (r15)
	 16: r16 (r16)
	 17: r17 (r17)
	 18: r18 (r18)
	 19: r19 (r19)
	 20: r20 (r20)
	 21: r21 (r21)
	 22: r22 (r22)
	 23: r23 (r23)
	 24: r24 (r24)
	 25: r25 (r25)
	 26: r26 (r26)
	 27: r27 (r27)
	 28: r28 (r28)
	 29: r29 (r29)
	 30: r30 (r30)
	 31: r31 (r31)
	 64: cr (cr)
	 66: msr (msr)
FPU registers:
	 32: f0 (f0)
	 33: f1 (f1)
	 34: f2 (f2)
	 35: f3 (f3)
	 36: f4 (f4)
	 37: f5 (f5)
	 38: f6 (f6)
	 39: f7 (f7)
	 40: f8 (f8)
	 41: f9 (f9)
	 42: f10 (f10)
	 43: f11 (f11)
	 44: f12 (f12)
	 45: f13 (f13)
	 46: f14 (f14)
	 47: f15 (f15)
	 48: f16 (f16)
	 49: f17 (f17)
	 50: f18 (f18)
	 51: f19 (f19)
	 52: f20 (f20)
	 53: f21 (f21)
	 54: f22 (f22)
	 55: f23 (f23)
	 56: f24 (f24)
	 57: f25 (f25)
	 58: f26 (f26)
	 59: f27 (f27)
	 60: f28 (f28)
	 61: f29 (f29)
	 62: f30 (f30)
	 63: f31 (f31)
	 65: fpscr (fpscr)
privileged registers:
	 70: sr0 (sr0)
	 71: sr1 (sr1)
	 72: sr2 (sr2)
	 73: sr3 (sr3)
	 74: sr4 (sr4)
	 75: sr5 (sr5)
	 76: sr6 (sr6)
	 77: sr7 (sr7)
	 78: sr8 (sr8)
	 79: sr9 (sr9)
	 80: sr10 (sr10)
	 81: sr11 (sr11)
	 82: sr12 (sr12)
	 83: sr13 (sr13)
	 84: sr14 (sr14)
	 85: sr15 (sr15)
	100: spr0 (spr0)
	101: spr1 (spr1)
	102: spr2 (spr2)
	103: spr3 (spr3)
	104: spr4 (spr4)
	105: spr5 (spr5)
	106: spr6 (spr6)
	107: spr7 (spr7)
	108: spr8 (spr8)
	109: spr9 (spr9)
	110: spr10 (spr10)
	111: spr11 (spr11)
	112: spr12 (spr12)
	113: spr13 (spr13)
	114: spr14 (spr14)
	115: spr15 (spr15)
	116: spr16 (spr16)
	117: spr17 (spr17)
	118: spr18 (spr18)
	119: spr19 (spr19)
	120: spr20 (spr20)
	121: spr21 (spr21)
	122: spr22 (spr22)
	123: spr23 (spr23)
	124: spr24 (spr24)
	125: spr25 (spr25)
	126: spr26 (spr26)
	127: spr27 (spr27)
	128: spr28 (spr28)
	129: spr29 (spr29)
	130: spr30 (spr30)
	131: spr31 (spr31)
	132: spr32 (spr32)
	133: spr33 (spr33)
	134: spr34 (spr34)
	135: spr35 (spr35)
	136: spr36 (spr36)
	137: spr37 (spr37)
	138: spr38 (spr38)
	139: spr39 (spr39)
	140: spr40 (spr40)
	141: spr41 (spr41)
	142: spr42 (spr42)
	143: spr43 (spr43)
	144: spr44 (spr44)
	145: spr45 (spr45)
	146: spr46 (spr46)
	147: spr47 (spr47)
	148: spr48 (spr48)
	149: spr49 (spr49)
	150: spr50 (spr50)
	151: spr51 (spr51)
	152: spr52 (spr52)
	153: spr53 (spr53)
	154: spr54 (spr54)
	155: spr55 (spr55)
	156: spr56 (spr56)
	157: spr57 (spr57)
	158: spr58 (spr58)
	159: spr59 (spr59)
	160: spr60 (spr60)
	161: spr61 (spr61)
	162: spr62 (spr62)
	163: spr63 (spr63)
	164: spr64 (spr64)
	165: spr65 (spr65)
	166: spr66 (spr66)
	167: spr67 (spr67)
	168: spr68 (spr68)
	169: spr69 (spr69)
	170: spr70 (spr70)
	171: spr71 (spr71)
	172: spr72 (spr72)
	173: spr73 (spr73)
	174: spr74 (spr74)
	175: spr75 (spr75)
	176: spr76 (spr76)
	177: spr77 (spr77)
	178: spr78 (spr78)
	179: spr79 (spr79)
	180: spr80 (spr80)
	181: spr81 (spr81)
	182: spr82 (spr82)
	183: spr83 (spr83)
	184: spr84 (spr84)
	185: spr85 (spr85)
	186: spr86 (spr86)
	187: spr87 (spr87)
	188: spr88 (spr88)
	189: spr89 (spr89)
	190: spr90 (spr90)
	191: spr91 (spr91)
	192: spr92 (spr92)
	193: spr93 (spr93)
	194: spr94 (spr94)
	195: spr95 (spr95)
	196: spr96 (spr96)
	197: spr97 (spr97)
	198: spr98 (spr98)
	199: spr99 (spr99)
	200: spr100 (spr100)
	201: spr101 (spr101)
	202: spr102 (spr102)
	203: spr103 (spr103)
	204: spr104 (spr104)
	205: spr105 (spr105)
	206: spr106 (spr106)
	207: spr107 (spr107)
	208: spr108 (spr108)
	209: spr109 (spr109)
	210: spr110 (spr110)
	211: spr111 (spr111)
	212: spr112 (spr112)
	213: spr113 (spr113)
	214: spr114 (spr114)
	215: spr115 (spr115)
	216: spr116 (spr116)
	217: spr117 (spr117)
	218: spr118 (spr118)
	219: spr119 (spr119)
	220: spr120 (spr120)
	221: spr121 (spr121)
	222: spr122 (spr122)
	223: spr123 (spr123)
	224: spr124 (spr124)
	225: spr125 (spr125)
	226: spr126 (spr126)
	227: spr127 (spr127)
	228: spr128 (spr128)
	229: spr129 (spr129)
	230: spr130 (spr130)
	231: spr131 (spr131)
	232: spr132 (spr132)
	233: spr133 (spr133)
	234: spr134 (spr134)
	235: spr135 (spr135)
	236: spr136 (spr136)
	237: spr137 (spr137)
	238: spr138 (spr138)
	239: spr139 (spr139)
	240: spr140 (spr140)
	241: spr141 (spr141)
	242: spr142 (spr142)
	243: spr143 (spr143)
	244: spr144 (spr144)
	245: spr145 (spr145)
	246: spr146 (spr146)
	247: spr147 (spr147)
	248: spr148 (spr148)
	249: spr149 (spr149)
	250: spr150 (spr150)
	251: spr151 (spr151)
	252: spr152 (spr152)
	253: spr153 (spr153)
	254: spr154 (spr154)
	255: spr155 (spr155)
	256: spr156 (spr156)
	257: spr157 (spr157)
	258: spr158 (spr158)
	259: spr159 (spr159)
	260: spr160 (spr160)
	261: spr161 (spr161)
	262: spr162 (spr162)
	263: spr163 (spr163)
	264: spr164 (spr164)
	265: spr165 (spr165)
	266: spr166 (spr166)
	267: spr167 (spr167)
	268: spr168 (spr168)
	269: spr169 (spr169)
	270: spr170 (spr170)
	271: spr171 (spr171)
	272: spr172 (spr172)
	273: spr173 (spr173)
	274: spr174 (spr174)
	275: spr175 (spr175)
	276: spr176 (spr176)
	277: spr177 (spr177)
	278: spr178 (spr178)
	279: spr179 (spr179)
	280: spr180 (spr180)
	281: spr181 (spr181)
	282: spr182 (spr182)
	283: spr183 (spr183)
	284: spr184 (spr184)
	285: spr185 (spr185)
	286: spr186 (spr186)
	287: spr187 (spr187)
	288: spr188 (spr188)
	289: spr189 (spr189)
	290: spr190 (spr190)
	291: spr191 (spr191)
	292: spr192 (spr192)
	293: spr193 (spr193)
	294: spr194 (spr194)
	295: spr195 (spr195)
	296: spr196 (spr196)
	297: spr197 (spr197)
	298: spr198 (spr198)
	299: spr199 (spr199)
	300: spr200 (spr200)
	301: spr201 (spr201)
	302: spr202 (spr202)
	303: spr203 (spr203)
	304: spr204 (spr204)
	305: spr205 (spr205)
	306: spr206 (spr206)
	307: spr207 (spr207)
	308: spr208 (spr208)
	309: spr209 (spr209)
	310: spr210 (spr210)
	311: spr211 (spr211)
	312: spr212 (spr212)
	313: spr213 (spr213)
	314: spr214 (spr214)
	315: spr215 (spr215)
	316: spr216 (spr216)
	317: spr217 (spr217)
	318: spr218 (spr218)
	319: spr219 (spr219)
	320: spr220 (spr220)
	321: spr221 (spr221)
	322: spr222 (spr222)
	323: spr223 (spr223)
	324: spr224 (spr224)
	325: spr225 (spr225)
	326: spr226 (spr226)
	327: spr227 (spr227)
	328: spr228 (spr228)
	329: spr229 (spr229)
	330: spr230 (spr230)
	331: spr231 (spr231)
	332: spr232 (spr232)
	333: spr233 (spr233)
	334: spr234 (spr234)
	335: spr235 (spr235)
	336: spr236 (spr236)
	337: spr237 (spr237)
	338: spr238 (spr238)
	339: spr239 (spr239)
	340: spr240 (spr240)
	341: spr241 (spr241)
	342: spr242 (spr242)
	343: spr243 (spr243)
	344: spr244 (spr244)
	345: spr245 (spr245)
	346: spr246 (spr246)
	347: spr247 (spr247)
	348: spr248 (spr248)
	349: spr249 (spr249)
	350: spr250 (spr250)
	351: spr251 (spr251)
	352: spr252 (spr252)
	353: spr253 (spr253)
	354: spr254 (spr254)
	355: spr255 (spr255)
	356: spr256 (spr256)
	357: spr257 (spr257)
	358: spr258 (spr258)
	359: spr259 (spr259)
	360: spr260 (spr260)
	361: spr261 (spr261)
	362: spr262 (spr262)
	363: spr263 (spr263)
	364: spr264 (spr264)
	365: spr265 (spr265)
	366: spr266 (spr266)
	367: spr267 (spr267)
	368: spr268 (spr268)
	369: spr269 (spr269)
	370: spr270 (spr270)
	371: spr271 (spr271)
	372: spr272 (spr272)
	373: spr273 (spr273)
	374: spr274 (spr274)
	375: spr275 (spr275)
	376: spr276 (spr276)
	377: spr277 (spr277)
	378: spr278 (spr278)
	379: spr279 (spr279)
	380: spr280 (spr280)
	381: spr281 (spr281)
	382: spr282 (spr282)
	383: spr283 (spr283)
	384: spr284 (spr284)
	385: spr285 (spr285)
	386: spr286 (spr286)
	387: spr287 (spr287)
	388: spr288 (spr288)
	389: spr289 (spr289)
	390: spr290 (spr290)
	391: spr291 (spr291)
	392: spr292 (spr292)
	393: spr293 (spr293)
	394: spr294 (spr294)
	395: spr295 (spr295)
	396: spr296 (spr296)
	397: spr297 (spr297)
	398: spr298 (spr298)
	399: spr299 (spr299)
	400: spr300 (spr300)
	401: spr301 (spr301)
	402: spr302 (spr302)
	403: spr303 (spr303)
	404: spr304 (spr304)
	405: spr305 (spr305)
	406: spr306 (spr306)
	407: spr307 (spr307)
	408: spr308 (spr308)
	409: spr309 (spr309)
	410: spr310 (spr310)
	411: spr311 (spr311)
	412: spr312 (spr312)
	413: spr313 (spr313)
	414: spr314 (spr314)
	415: spr315 (spr315)
	416: spr316 (spr316)
	417: spr317 (spr317)
	418: spr318 (spr318)
	419: spr319 (spr319)
	420: spr320 (spr320)
	421: spr321 (spr321)
	422: spr322 (spr322)
	423: spr323 (spr323)
	424: spr324 (spr324)
	425: spr325 (spr325)
	426: spr326 (spr326)
	427: spr327 (spr327)
	428: spr328 (spr328)
	429: spr329 (spr329)
	430: spr330 (spr330)
	431: spr331 (spr331)
	432: spr332 (spr332)
	433: spr333 (spr333)
	434: spr334 (spr334)
	435: spr335 (spr335)
	436: spr336 (spr336)
	437: spr337 (spr337)
	438: spr338 (spr338)
	439: spr339 (spr339)
	440: spr340 (spr340)
	441: spr341 (spr341)
	442: spr342 (spr342)
	443: spr343 (spr343)
	444: spr344 (spr344)
	445: spr345 (spr345)
	446: spr346 (spr346)
	447: spr347 (spr347)
	448: spr348 (spr348)
	449: spr349 (spr349)
	450: spr350 (spr350)
	451: spr351 (spr351)
	452: spr352 (spr352)
	453: spr353 (spr353)
	454: spr354 (spr354)
	455: spr355 (spr355)
	456: spr356 (spr356)
	457: spr357 (spr357)
	458: spr358 (spr358)
	459: spr359 (spr359)
	460: spr360 (spr360)
	461: spr361 (spr361)
	462: spr362 (spr362)
	463: spr363 (spr363)
	464: spr364 (spr364)
	465: spr365 (spr365)
	466: spr366 (spr366)
	467: spr367 (spr367)
	468: spr368 (spr368)
	469: spr369 (spr369)
	470: spr370 (spr370)
	471: spr371 (spr371)
	472: spr372 (spr372)
	473: spr373 (spr373)
	474: spr374 (spr374)
	475: spr375 (spr375)
	476: spr376 (spr376)
	477: spr377 (spr377)
	478: spr378 (spr378)
	479: spr379 (spr379)
	480: spr380 (spr380)
	481: spr381 (spr381)
	482: spr382 (spr382)
	483: spr383 (spr383)
	484: spr384 (spr384)
	485: spr385 (spr385)
	486: spr386 (spr386)
	487: spr387 (spr387)
	488: spr388 (spr388)
	489: spr389 (spr389)
	490: spr390 (spr390)
	491: spr391 (spr391)
	492: spr392 (spr392)
	493: spr393 (spr393)
	494: spr394 (spr394)
	495: spr395 (spr395)
	496: spr396 (spr396)
	497: spr397 (spr397)
	498: spr398 (spr398)
	499: spr399 (spr399)
	500: spr400 (spr400)
	501: spr401 (spr401)
	502: spr402 (spr402)
	503: spr403 (spr403)
	504: spr404 (spr404)
	505: spr405 (spr405)
	506: spr406 (spr406)
	507: spr407 (spr407)
	508: spr408 (spr408)
	509: spr409 (spr409)
	510: spr410 (spr410)
	511: spr411 (spr411)
	512: spr412 (spr412)
	513: spr413 (spr413)
	514: spr414 (spr414)
	515: spr415 (spr415)
	516: spr416 (spr416)
	517: spr417 (spr417)
	518: spr418 (spr418)
	519: spr419 (spr419)
	520: spr420 (spr420)
	521: spr421 (spr421)
	522: spr422 (spr422)
	523: spr423 (spr423)
	524: spr424 (spr424)
	525: spr425 (spr425)
	526: spr426 (spr426)
	527: spr427 (spr427)
	528: spr428 (spr428)
	529: spr429 (spr429)
	530: spr430 (spr430)
	531: spr431 (spr431)
	532: spr432 (spr432)
	533: spr433 (spr433)
	534: spr434 (spr434)
	535: spr435 (spr435)
	536: spr436 (spr436)
	537: spr437 (spr437)
	538: spr438 (spr438)
	539: spr439 (spr439)
	540: spr440 (spr440)
	541: spr441 (spr441)
	542: spr442 (spr442)
	543: spr443 (spr443)
	544: spr444 (spr444)
	545: spr445 (spr445)
	546: spr446 (spr446)
	547: spr447 (spr447)
	548: spr448 (spr448)
	549: spr449 (spr449)
	550: spr450 (spr450)
	551: spr451 (spr451)
	552: spr452 (spr452)
	553: spr453 (spr453)
	554: spr454 (spr454)
	555: spr455 (spr455)
	556: spr456 (spr456)
	557: spr457 (spr457)
	558: spr458 (spr458)
	559: spr459 (spr459)
	560: spr460 (spr460)
	561: spr461 (spr461)
	562: spr462 (spr462)
	563: spr463 (spr463)
	564: spr464 (spr464)
	565: spr465 (spr465)
	566: spr466 (spr466)
	567: spr467 (spr467)
	568: spr468 (spr468)
	569: spr469 (spr469)
	570: spr470 (spr470)
	571: spr471 (spr471)
	572: spr472 (spr472)
	573: spr473 (spr473)
	574: spr474 (spr474)
	575: spr475 (spr475)
	576: spr476 (spr476)
	577: spr477 (spr477)
	578: spr478 (spr478)
	579: spr479 (spr479)
	580: spr480 (spr480)
	581: spr481 (spr481)
	582: spr482 (spr482)
	583: spr483 (spr483)
	584: spr484 (spr484)
	585: spr485 (spr485)
	586: spr486 (spr486)
	587: spr487 (spr487)
	588: spr488 (spr488)
	589: spr489 (spr489)
	590: spr490 (spr490)
	591: spr491 (spr491)
	592: spr492 (spr492)
	593: spr493 (spr493)
	594: spr494 (spr494)
	595: spr495 (spr495)
	596: spr496 (spr496)
	597: spr497 (spr497)
	598: spr498 (spr498)
	599: spr499 (spr499)
	600: spr500 (spr500)
	601: spr501 (spr501)
	602: spr502 (spr502)
	603: spr503 (spr503)
	604: spr504 (spr504)
	605: spr505 (spr505)
	606: spr506 (spr506)
	607: spr507 (spr507)
	608: spr508 (spr508)
	609: spr509 (spr509)
	610: spr510 (spr510)
	611: spr511 (spr511)
	612: spr512 (spr512)
	613: spr513 (spr513)
	614: spr514 (spr514)
	615: spr515 (spr515)
	616: spr516 (spr516)
	617: spr517 (spr517)
	618: spr518 (spr518)
	619: spr519 (spr519)
	620: spr520 (spr520)
	621: spr521 (spr521)
	622: spr522 (spr522)
	623: spr523 (spr523)
	624: spr524 (spr524)
	625: spr525 (spr525)
	626: spr526 (spr526)
	627: spr527 (spr527)
	628: spr528 (spr528)
	629: spr529 (spr529)
	630: spr530 (spr530)
	631: spr531 (spr531)
	632: spr532 (spr532)
	633: spr533 (spr533)
	634: spr534 (spr534)
	635: spr535 (spr535)
	636: spr536 (spr536)
	637: spr537 (spr537)
	638: spr538 (spr538)
	639: spr539 (spr539)
	640: spr540 (spr540)
	641: spr541 (spr541)
	642: spr542 (spr542)
	643: spr543 (spr543)
	644: spr544 (spr544)
	645: spr545 (spr545)
	646: spr546 (spr546)
	647: spr547 (spr547)
	648: spr548 (spr548)
	649: spr549 (spr549)
	650: spr550 (spr550)
	651: spr551 (spr551)
	652: spr552 (spr552)
	653: spr553 (spr553)
	654: spr554 (spr554)
	655: spr555 (spr555)
	656: spr556 (spr556)
	657: spr557 (spr557)
	658: spr558 (spr558)
	659: spr559 (spr559)
	660: spr560 (spr560)
	661: spr561 (spr561)
	662: spr562 (spr562)
	663: spr563 (spr563)
	664: spr564 (spr564)
	665: spr565 (spr565)
	666: spr566 (spr566)
	667: spr567 (spr567)
	668: spr568 (spr568)
	669: spr569 (spr569)
	670: spr570 (spr570)
	671: spr571 (spr571)
	672: spr572 (spr572)
	673: spr573 (spr573)
	674: spr574 (spr574)
	675: spr575 (spr575)
	676: spr576 (spr576)
	677: spr577 (spr577)
	678: spr578 (spr578)
	679: spr579 (spr579)
	680: spr580 (spr580)
	681: spr581 (spr581)
	682: spr582 (spr582)
	683: spr583 (spr583)
	684: spr584 (spr584)
	685: spr585 (spr585)
	686: spr586 (spr586)
	687: spr587 (spr587)
	688: spr588 (spr588)
	689: spr589 (spr589)
	690: spr590 (spr590)
	691: spr591 (spr591)
	692: spr592 (spr592)
	693: spr593 (spr593)
	694: spr594 (spr594)
	695: spr595 (spr595)
	696: spr596 (spr596)
	697: spr597 (spr597)
	698: spr598 (spr598)
	699: spr599 (spr599)
	700: spr600 (spr600)
	701: spr601 (spr601)
	702: spr602 (spr602)
	703: spr603 (spr603)
	704: spr604 (spr604)
	705: spr605 (spr605)
	706: spr606 (spr606)
	707: spr607 (spr607)
	708: spr608 (spr608)
	709: spr609 (spr609)
	710: spr610 (spr610)
	711: spr611 (spr611)
	712: spr612 (spr612)
	713: spr613 (spr613)
	714: spr614 (spr614)
	715: spr615 (spr615)
	716: spr616 (spr616)
	717: spr617 (spr617)
	718: spr618 (spr618)
	719: spr619 (spr619)
	720: spr620 (spr620)
	721: spr621 (spr621)
	722: spr622 (spr622)
	723: spr623 (spr623)
	724: spr624 (spr624)
	725: spr625 (spr625)
	726: spr626 (spr626)
	727: spr627 (spr627)
	728: spr628 (spr628)
	729: spr629 (spr629)
	730: spr630 (spr630)
	731: spr631 (spr631)
	732: spr632 (spr632)
	733: spr633 (spr633)
	734: spr634 (spr634)
	735: spr635 (spr635)
	736: spr636 (spr636)
	737: spr637 (spr637)
	738: spr638 (spr638)
	739: spr639 (spr639)
	740: spr640 (spr640)
	741: spr641 (spr641)
	742: spr642 (spr642)
	743: spr643 (spr643)
	744: spr644 (spr644)
	745: spr645 (spr645)
	746: spr646 (spr646)
	747: spr647 (spr647)
	748: spr648 (spr648)
	749: spr649 (spr649)
	750: spr650 (spr650)
	751: spr651 (spr651)
	752: spr652 (spr652)
	753: spr653 (spr653)
	754: spr654 (spr654)
	755: spr655 (spr655)
	756: spr656 (spr656)
	757: spr657 (spr657)
	758: spr658 (spr658)
	759: spr659 (spr659)
	760: spr660 (spr660)
	761: spr661 (spr661)
	762: spr662 (spr662)
	763: spr663 (spr663)
	764: spr664 (spr664)
	765: spr665 (spr665)
	766: spr666 (spr666)
	767: spr667 (spr667)
	768: spr668 (spr668)
	769: spr669 (spr669)
	770: spr670 (spr670)
	771: spr671 (spr671)
	772: spr672 (spr672)
	773: spr673 (spr673)
	774: spr674 (spr674)
	775: spr675 (spr675)
	776: spr676 (spr676)
	777: spr677 (spr677)
	778: spr678 (spr678)
	779: spr679 (spr679)
	780: spr680 (spr680)
	781: spr681 (spr681)
	782: spr682 (spr682)
	783: spr683 (spr683)
	784: spr684 (spr684)
	785: spr685 (spr685)
	786: spr686 (spr686)
	787: spr687 (spr687)
	788: spr688 (spr688)
	789: spr689 (spr689)
	790: spr690 (spr690)
	791: spr691 (spr691)
	792: spr692 (spr692)
	793: spr693 (spr693)
	794: spr694 (spr694)
	795: spr695 (spr695)
	796: spr696 (spr696)
	797: spr697 (spr697)
	798: spr698 (spr698)
	799: spr699 (spr699)
	800: spr700 (spr700)
	801: spr701 (spr701)
	802: spr702 (spr702)
	803: spr703 (spr703)
	804: spr704 (spr704)
	805: spr705 (spr705)
	806: spr706 (spr706)
	807: spr707 (spr707)
	808: spr708 (spr708)
	809: spr709 (spr709)
	810: spr710 (spr710)
	811: spr711 (spr711)
	812: spr712 (spr712)
	813: spr713 (spr713)
	814: spr714 (spr714)
	815: spr715 (spr715)
	816: spr716 (spr716)
	817: spr717 (spr717)
	818: spr718 (spr718)
	819: spr719 (spr719)
	820: spr720 (spr720)
	821: spr721 (spr721)
	822: spr722 (spr722)
	823: spr723 (spr723)
	824: spr724 (spr724)
	825: spr725 (spr725)
	826: spr726 (spr726)
	827: spr727 (spr727)
	828: spr728 (spr728)
	829: spr729 (spr729)
	830: spr730 (spr730)
	831: spr731 (spr731)
	832: spr732 (spr732)
	833: spr733 (spr733)
	834: spr734 (spr734)
	835: spr735 (spr735)
	836: spr736 (spr736)
	837: spr737 (spr737)
	838: spr738 (spr738)
	839: spr739 (spr739)
	840: spr740 (spr740)
	841: spr741 (spr741)
	842: spr742 (spr742)
	843: spr743 (spr743)
	844: spr744 (spr744)
	845: spr745 (spr745)
	846: spr746 (spr746)
	847: spr747 (spr747)
	848: spr748 (spr748)
	849: spr749 (spr749)
	850: spr750 (spr750)
	851: spr751 (spr751)
	852: spr752 (spr752)
	853: spr753 (spr753)
	854: spr754 (spr754)
	855: spr755 (spr755)
	856: spr756 (spr756)
	857: spr757 (spr757)
	858: spr758 (spr758)
	859: spr759 (spr759)
	860: spr760 (spr760)
	861: spr761 (spr761)
	862: spr762 (spr762)
	863: spr763 (spr763)
	864: spr764 (spr764)
	865: spr765 (spr765)
	866: spr766 (spr766)
	867: spr767 (spr767)
	868: spr768 (spr768)
	869: spr769 (spr769)
	870: spr770 (spr770)
	871: spr771 (spr771)
	872: spr772 (spr772)
	873: spr773 (spr773)
	874: spr774 (spr774)
	875: spr775 (spr775)
	876: spr776 (spr776)
	877: spr777 (spr777)
	878: spr778 (spr778)
	879: spr779 (spr779)
	880: spr780 (spr780)
	881: spr781 (spr781)
	882: spr782 (spr782)
	883: spr783 (spr783)
	884: spr784 (spr784)
	885: spr785 (spr785)
	886: spr786 (spr786)
	887: spr787 (spr787)
	888: spr788 (spr788)
	889: spr789 (spr789)
	890: spr790 (spr790)
	891: spr791 (spr791)
	892: spr792 (spr792)
	893: spr793 (spr793)
	894: spr794 (spr794)
	895: spr795 (spr795)
	896: spr796 (spr796)
	897: spr797 (spr797)
	898: spr798 (spr798)
	899: spr799 (spr799)
	900: spr800 (spr800)
	901: spr801 (spr801)
	902: spr802 (spr802)
	903: spr803 (spr803)
	904: spr804 (spr804)
	905: spr805 (spr805)
	906: spr806 (spr806)
	907: spr807 (spr807)
	908: spr808 (spr808)
	909: spr809 (spr809)
	910: spr810 (spr810)
	911: spr811 (spr811)
	912: spr812 (spr812)
	913: spr813 (spr813)
	914: spr814 (spr814)
	915: spr815 (spr815)
	916: spr816 (spr816)
	917: spr817 (spr817)
	918: spr818 (spr818)
	919: spr819 (spr819)
	920: spr820 (spr820)
	921: spr821 (spr821)
	922: spr822 (spr822)
	923: spr823 (spr823)
	924: spr824 (spr824)
	925: spr825 (spr825)
	926: spr826 (spr826)
	927: spr827 (spr827)
	928: spr828 (spr828)
	929: spr829 (spr829)
	930: spr830 (spr830)
	931: spr831 (spr831)
	932: spr832 (spr832)
	933: spr833 (spr833)
	934: spr834 (spr834)
	935: spr835 (spr835)
	936: spr836 (spr836)
	937: spr837 (spr837)
	938: spr838 (spr838)
	939: spr839 (spr839)
	940: spr840 (spr840)
	941: spr841 (spr841)
	942: spr842 (spr842)
	943: spr843 (spr843)
	944: spr844 (spr844)
	945: spr845 (spr845)
	946: spr846 (spr846)
	947: spr847 (spr847)
	948: spr848 (spr848)
	949: spr849 (spr849)
	950: spr850 (spr850)
	951: spr851 (spr851)
	952: spr852 (spr852)
	953: spr853 (spr853)
	954: spr854 (spr854)
	955: spr855 (spr855)
	956: spr856 (spr856)
	957: spr857 (spr857)
	958: spr858 (spr858)
	959: spr859 (spr859)
	960: spr860 (spr860)
	961: spr861 (spr861)
	962: spr862 (spr862)
	963: spr863 (spr863)
	964: spr864 (spr864)
	965: spr865 (spr865)
	966: spr866 (spr866)
	967: spr867 (spr867)
	968: spr868 (spr868)
	969: spr869 (spr869)
	970: spr870 (spr870)
	971: spr871 (spr871)
	972: spr872 (spr872)
	973: spr873 (spr873)
	974: spr874 (spr874)
	975: spr875 (spr875)
	976: spr876 (spr876)
	977: spr877 (spr877)
	978: spr878 (spr878)
	979: spr879 (spr879)
	980: spr880 (spr880)
	981: spr881 (spr881)
	982: spr882 (spr882)
	983: spr883 (spr883)
	984: spr884 (spr884)
	985: spr885 (spr885)
	986: spr886 (spr886)
	987: spr887 (spr887)
	988: spr888 (spr888)
	989: spr889 (spr889)
	990: spr890 (spr890)
	991: spr891 (spr891)
	992: spr892 (spr892)
	993: spr893 (spr893)
	994: spr894 (spr894)
	995: spr895 (spr895)
	996: spr896 (spr896)
	997: spr897 (spr897)
	998: spr898 (spr898)
	999: spr899 (spr899)
vector registers:
	1124: vr0 (vr0)
	1125: vr1 (vr1)
	1126: vr2 (vr2)
	1127: vr3 (vr3)
	1128: vr4 (vr4)
	1129: vr5 (vr5)
	1130: vr6 (vr6)
	1131: vr7 (vr7)
	1132: vr8 (vr8)
	1133: vr9 (vr9)
	1134: vr10 (vr10)
	1135: vr11 (vr11)
	1136: vr12 (vr12)
	1137: vr13 (vr13)
	1138: vr14 (vr14)
	1139: vr15 (vr15)
	1140: vr16 (vr16)
	1141: vr17 (vr17)
	1142: vr18 (vr18)
	1143: vr19 (vr19)
	1144: vr20 (vr20)
	1145: vr21 (vr21)
	1146: vr22 (vr22)
	1147: vr23 (vr23)
	1148: vr24 (vr24)
	1149: vr25 (vr25)
	1150: vr26 (vr26)
	1151: vr27 (vr27)
	1152: vr28 (vr28)
	1153: vr29 (vr29)
	1154: vr30 (vr30)
	1155: vr31 (vr31)
EOF

regs_test testfile26 testfile27 <<\EOF
integer registers:
	  0: %r0 (r0)
	  1: %r1 (r1)
	  2: %r2 (r2)
	  3: %r3 (r3)
	  4: %r4 (r4)
	  5: %r5 (r5)
	  6: %r6 (r6)
	  7: %r7 (r7)
	  8: %r8 (r8)
	  9: %r9 (r9)
	 10: %r10 (r10)
	 11: %r11 (r11)
	 12: %r12 (r12)
	 13: %r13 (r13)
	 14: %r14 (r14)
	 15: %r15 (r15)
FPU registers:
	 16: %f0 (f0)
	 17: %f2 (f2)
	 18: %f4 (f4)
	 19: %f6 (f6)
	 20: %f1 (f1)
	 21: %f3 (f3)
	 22: %f5 (f5)
	 23: %f7 (f7)
	 24: %f8 (f8)
	 25: %f10 (f10)
	 26: %f12 (f12)
	 27: %f14 (f14)
	 28: %f9 (f9)
	 29: %f11 (f11)
	 30: %f13 (f13)
	 31: %f15 (f15)
access registers:
	 48: %a0 (a0)
	 49: %a1 (a1)
	 50: %a2 (a2)
	 51: %a3 (a3)
	 52: %a4 (a4)
	 53: %a5 (a5)
	 54: %a6 (a6)
	 55: %a7 (a7)
	 56: %a8 (a8)
	 57: %a9 (a9)
	 58: %a10 (a10)
	 59: %a11 (a11)
	 60: %a12 (a12)
	 61: %a13 (a13)
	 62: %a14 (a14)
	 63: %a15 (a15)
control registers:
	 32: %c0 (c0)
	 33: %c1 (c1)
	 34: %c2 (c2)
	 35: %c3 (c3)
	 36: %c4 (c4)
	 37: %c5 (c5)
	 38: %c6 (c6)
	 39: %c7 (c7)
	 40: %c8 (c8)
	 41: %c9 (c9)
	 42: %c10 (c10)
	 43: %c11 (c11)
	 44: %c12 (c12)
	 45: %c13 (c13)
	 46: %c14 (c14)
	 47: %c15 (c15)
	 64: %pswm (pswm)
	 65: %pswa (pswa)
EOF

exit 0
