/*
 * Authors :
 *   Pierre DAVID (pda@masi.ibp.fr or pda@frunip62.bitnet)
 *   Janick TAILLANDIER
 *
 * This program can be freely used or distributed as long as this
 * note is kept.
 *
 * This program is provided "as is".
 */

#include "aglobal.h"

struct mnemo_desc mnemo_table [] = {

/* 000 */	{1, "?A>B", 3, 3, "800", 2, 0},
/* 001 */	{5, "CR1EX", 5, 3, "129", 0, 0},
/* 002 */	{5, "AR0EX", 5, 3, "120", 0, 0},
/* 003 */	{10, "P=C", 33, 4, "80D0", 0, 0},
/* 004 */	{5, "SETDEC", 5, 2, "05", 0, 0},
/* 005 */	{3, "C=C!B", 1, 4, "0E0D", 0, 0},
/* 006 */	{5, "CSLC", 1, 3, "812", 0, 0},
/* 007 */	{5, "D1=CS", 5, 3, "13D", 0, 0},
/* 008 */	{40, "ENDIF", 0, 0, "", 0, 0},
/* 009 */	{4, "GOYES", 41, 2, "00", 1, 2},
/* 010 */	{4, "GOSBVL", 53, 7, "8F000", 3, 5},
/* 011 */	{13, "DAT0=A", 32, 4, "1500", 0, 0},
/* 012 */	{4, "GONC", 33, 3, "500", 2, 2},
/* 013 */	{22, "EQU", 36, 0, "", 0, 0},
/* 014 */	{1, "?C>A", 3, 3, "802", 2, 0},
/* 015 */	{1, "?A>=C", 3, 3, "80E", 2, 0},
/* 016 */	{1, "?B#C", 7, 3, "805", 1, 0},
/* 017 */	{1, "?A<=B", 7, 3, "80C", 2, 0},
/* 018 */	{2, "B=B+B", 4, 3, "005", 1, 0},
/* 019 */	{5, "RTNC", 5, 3, "400", 0, 0},
/* 020 */	{33, "CHAR", 32, 1, "0", 0, 0},
/* 021 */	{5, "D1=AS", 5, 3, "138", 0, 0},
/* 022 */	{5, "NOP4", 5, 4, "6300", 0, 0},
/* 023 */	{5, "RTI", 1, 2, "0F", 0, 0},
/* 024 */	{2, "A=C", 4, 3, "00A", 2, 0},
/* 025 */	{7, "RTNYES", 13, 2, "00", 0, 0},
/* 026 */	{2, "A=A-B", 0, 3, "000", 3, 0},
/* 027 */	{1, "?B#A", 7, 3, "804", 1, 0},
/* 028 */	{5, "RTNNC", 1, 3, "500", 0, 0},
/* 029 */	{2, "C=D", 0, 3, "00B", 2, 0},
/* 030 */	{5, "DSLC", 5, 3, "813", 0, 0},
/* 031 */	{8, "?P#", 39, 3, "880", 0, 0},
/* 032 */	{6, "?MP=0", 3, 3, "838", 0, 0},
/* 033 */	{13, "DAT0=C", 32, 4, "1540", 0, 0},
/* 034 */	{2, "A=A+B", 0, 3, "000", 1, 0},
/* 035 */	{2, "C=D+C", 4, 3, "00B", 1, 0},
/* 036 */	{1, "?D#0", 7, 3, "80F", 1, 0},
/* 037 */	{29, "ID", 36, 16, "", 0, 0},
/* 038 */	{27, "WORDI", 32, 0, "", 0, 0},
/* 039 */	{4, "LC(6)", 177, 8, "35000", 3, 6},
/* 040 */	{2, "B=C", 4, 3, "005", 2, 0},
/* 041 */	{5, "R0=C", 5, 3, "108", 0, 0},
/* 042 */	{5, "CR0EX", 1, 3, "128", 0, 0},
/* 043 */	{1, "?A=B", 7, 3, "800", 1, 0},
/* 044 */	{2, "ABEX", 4, 3, "00C", 2, 0},
/* 045 */	{1, "?B<=A", 3, 3, "808", 2, 0},
/* 046 */	{2, "D=D-C", 4, 3, "003", 3, 0},
/* 047 */	{3, "C=D!C", 1, 4, "0E0F", 0, 0},
/* 048 */	{3, "A=A!B", 5, 4, "0E08", 0, 0},
/* 049 */	{5, "ASRC", 1, 3, "814", 0, 0},
/* 050 */	{4, "LC(4)", 177, 6, "33000", 3, 4},
/* 051 */	{5, "C=ST", 5, 2, "09", 0, 0},
/* 052 */	{5, "A=R4", 1, 3, "114", 0, 0},
/* 053 */	{1, "?C=A", 3, 3, "802", 1, 0},
/* 054 */	{1, "?A=0", 7, 3, "808", 1, 0},
/* 055 */	{5, "R0=A", 1, 3, "100", 0, 0},
/* 056 */	{2, "B=A+B", 0, 3, "008", 1, 0},
/* 057 */	{4, "LC(2)", 177, 4, "3100", 3, 2},
/* 058 */	{5, "A=R2", 5, 3, "112", 0, 0},
/* 059 */	{23, "LIST", 5, 0, "", 0, 0},
/* 060 */	{2, "D=D+C", 4, 3, "003", 1, 0},
/* 061 */	{4, "D0=(2)", 177, 4, "1900", 3, 2},
/* 062 */	{5, "R4=C", 5, 3, "10C", 0, 0},
/* 063 */	{5, "BUSCC", 5, 3, "80B", 0, 0},
/* 064 */	{5, "A=R0", 1, 3, "110", 0, 0},
/* 065 */	{2, "A=B", 4, 3, "004", 2, 0},
/* 066 */	{4, "CON(4)", 49, 4, "0000", 1, 4},
/* 067 */	{3, "B=A!B", 1, 4, "0E0C", 0, 0},
/* 068 */	{5, "RTNSXM", 5, 2, "00", 0, 0},
/* 069 */	{3, "D=D!C", 5, 4, "0E0B", 0, 0},
/* 070 */	{5, "BSRC", 5, 3, "815", 0, 0},
/* 071 */	{5, "R4=A", 1, 3, "104", 0, 0},
/* 072 */	{4, "GOTO", 33, 4, "6000", 2, 3},
/* 073 */	{1, "?C>=A", 7, 3, "80A", 2, 0},
/* 074 */	{2, "C=C+C", 4, 3, "006", 1, 0},
/* 075 */	{9, "?ST=0", 35, 3, "860", 0, 0},
/* 076 */	{3, "A=C&A", 5, 4, "0E06", 0, 0},
/* 077 */	{5, "ST=C", 5, 2, "0A", 0, 0},
/* 078 */	{5, "A=IN", 1, 3, "802", 0, 0},
/* 079 */	{13, "A=DAT0", 36, 4, "1520", 0, 0},
/* 080 */	{5, "D0=C", 5, 3, "134", 0, 0},
/* 081 */	{12, "D1=D1+", 33, 3, "170", 0, 0},
/* 082 */	{5, "CSRC", 1, 3, "816", 0, 0},
/* 083 */	{1, "?A<B", 7, 3, "804", 2, 0},
/* 084 */	{9, "?ST#0", 35, 3, "870", 0, 0},
/* 085 */	{11, "ST=1", 33, 3, "850", 0, 0},
/* 086 */	{2, "CBEX", 0, 3, "00D", 2, 0},
/* 087 */	{2, "B=B-C", 0, 3, "001", 3, 0},
/* 088 */	{2, "D=C", 4, 3, "007", 2, 0},
/* 089 */	{5, "CLRST", 5, 2, "08", 0, 0},
/* 090 */	{5, "SB=0", 1, 3, "822", 0, 0},
/* 091 */	{5, "C=R4", 5, 3, "11C", 0, 0},
/* 092 */	{5, "SR=0", 5, 3, "824", 0, 0},
/* 093 */	{13, "A=DAT1", 36, 4, "1530", 0, 0},
/* 094 */	{5, "D0=A", 1, 3, "130", 0, 0},
/* 095 */	{1, "?A<=C", 7, 3, "80A", 2, 0},
/* 096 */	{1, "?C<A", 3, 3, "806", 2, 0},
/* 097 */	{2, "B=B+C", 0, 3, "001", 1, 0},
/* 098 */	{5, "C=R2", 5, 3, "11A", 0, 0},
/* 099 */	{4, "REL(1)", 37, 1, "0", 1, 1},
/* 100 */	{12, "D1=D1-", 37, 3, "1C0", 0, 0},
/* 101 */	{2, "C=B", 4, 3, "009", 2, 0},
/* 102 */	{5, "C=R0", 1, 3, "118", 0, 0},
/* 103 */	{2, "A=A-C", 4, 3, "00A", 3, 0},
/* 104 */	{3, "B=B!C", 1, 4, "0E09", 0, 0},
/* 105 */	{5, "DSRC", 1, 3, "817", 0, 0},
/* 106 */	{5, "OUT=C", 5, 3, "801", 0, 0},
/* 107 */	{2, "A=A+C", 0, 3, "00A", 1, 0},
/* 108 */	{2, "C=B+C", 0, 3, "009", 1, 0},
/* 109 */	{3, "C=C&A", 5, 4, "0E02", 0, 0},
/* 110 */	{3, "A=B&A", 5, 4, "0E00", 0, 0},
/* 111 */	{1, "?D>C", 7, 3, "803", 2, 0},
/* 112 */	{35, "TOKEN", 36, 2, "00", 0, 0},
/* 113 */	{5, "C=IN", 1, 3, "803", 0, 0},
/* 114 */	{13, "C=DAT0", 32, 4, "1560", 0, 0},
/* 115 */	{14, "NIBHEX", 4, 0, "", 1, 14},
/* 116 */	{10, "P=", 33, 2, "20", 0, 0},
/* 117 */	{2, "B=A", 0, 3, "008", 2, 0},
/* 118 */	{1, "?A#B", 7, 3, "804", 1, 0},
/* 119 */	{1, "?B>=C", 3, 3, "809", 2, 0},
/* 120 */	{3, "A=A!C", 5, 4, "0E0E", 0, 0},
/* 121 */	{3, "C=B!C", 5, 4, "0E0D", 0, 0},
/* 122 */	{5, "C=ID", 1, 3, "806", 0, 0},
/* 123 */	{5, "RESET", 5, 3, "80A", 0, 0},
/* 124 */	{41, "ABS", 0, 0, "", 0, 0},
/* 125 */	{13, "C=DAT1", 36, 4, "1570", 0, 0},
/* 126 */	{1, "?C#A", 3, 3, "806", 1, 0},
/* 127 */	{1, "?A#0", 7, 3, "80C", 1, 0},
/* 128 */	{2, "D=D+D", 0, 3, "007", 1, 0},
/* 129 */	{3, "B=B&A", 1, 4, "0E04", 0, 0},
/* 130 */	{6, "?XM=0", 3, 3, "831", 0, 0},
/* 131 */	{2, "A=-A-1", 4, 3, "00C", 4, 0},
/* 132 */	{5, "OUT=CS", 5, 3, "800", 0, 0},
/* 133 */	{42, "RDSYMB", 0, 0, "", 0, 0},
/* 134 */	{2, "C=A", 0, 3, "006", 2, 0},
/* 135 */	{2, "A=0", 4, 3, "000", 2, 0},
/* 136 */	{2, "C=A-C", 4, 3, "00E", 3, 0},
/* 137 */	{4, "CON(5)", 53, 5, "00000", 1, 5},
/* 138 */	{34, "KEY", 32, 0, "", 0, 0},
/* 139 */	{2, "C=C-D", 0, 3, "00B", 3, 0},
/* 140 */	{2, "C=A+C", 4, 3, "002", 1, 0},
/* 141 */	{5, "R1=C", 5, 3, "109", 0, 0},
/* 142 */	{2, "ACEX", 0, 3, "00E", 2, 0},
/* 143 */	{1, "?C<=A", 7, 3, "80E", 2, 0},
/* 144 */	{1, "?C>=B", 3, 3, "80D", 2, 0},
/* 145 */	{1, "?D=C", 7, 3, "803", 1, 0},
/* 146 */	{2, "C=C+D", 0, 3, "00B", 1, 0},
/* 147 */	{9, "?ST=1", 35, 3, "870", 0, 0},
/* 148 */	{2, "A=-A", 4, 3, "008", 4, 0},
/* 149 */	{30, "MSG", 32, 4, "0000", 0, 0},
/* 150 */	{6, "?SB=0", 3, 3, "832", 0, 0},
/* 151 */	{6, "?SR=0", 7, 3, "834", 0, 0},
/* 152 */	{5, "R1=A", 5, 3, "101", 0, 0},
/* 153 */	{2, "B=0", 4, 3, "001", 2, 0},
/* 154 */	{9, "?ST#1", 35, 3, "860", 0, 0},
/* 155 */	{1, "?B=0", 7, 3, "809", 1, 0},
/* 156 */	{4, "D1=(2)", 177, 4, "1D00", 3, 2},
/* 157 */	{2, "D=C-D", 4, 3, "00F", 3, 0},
/* 158 */	{3, "C=A!C", 5, 4, "0E0A", 0, 0},
/* 159 */	{2, "C=-C-1", 0, 3, "00E", 4, 0},
/* 160 */	{3, "C=C!D", 1, 4, "0E0F", 0, 0},
/* 161 */	{31, "POLL", 32, 5, "00000", 0, 0},
/* 162 */	{15, "LCHEX", 4, 2, "30", 3, 2},
/* 163 */	{2, "D=C+D", 4, 3, "003", 1, 0},
/* 164 */	{2, "BCEX", 0, 3, "00D", 2, 0},
/* 165 */	{3, "B=C&B", 5, 4, "0E01", 0, 0},
/* 166 */	{4, "REL(2)", 33, 2, "00", 1, 2},
/* 167 */	{21, "END", 0, 0, "", 0, 0},
/* 168 */	{2, "C=0", 4, 3, "002", 2, 0},
/* 169 */	{5, "AD1EX", 5, 3, "133", 0, 0},
/* 170 */	{10, "C=P", 33, 4, "80C0", 0, 0},
/* 171 */	{2, "C=-C", 0, 3, "00A", 4, 0},
/* 172 */	{3, "D=C!D", 5, 4, "0E0B", 0, 0},
/* 173 */	{43, "ELSE", 0, 0, "", 0, 0},
/* 174 */	{4, "GOLONG", 33, 6, "8C000", 3, 4},
/* 175 */	{5, "D1=C", 1, 3, "135", 0, 0},
/* 176 */	{1, "?D<C", 7, 3, "807", 2, 0},
/* 177 */	{3, "C=C&B", 5, 4, "0E05", 0, 0},
/* 178 */	{2, "ASL", 0, 3, "000", 4, 0},
/* 179 */	{39, "ENDTXT", 4, 3, "", 0, 0},
/* 180 */	{5, "D1=A", 1, 3, "131", 0, 0},
/* 181 */	{17, "NIBASC", 36, 0, "", 1, 14},
/* 182 */	{1, "?B<=C", 7, 3, "80D", 2, 0},
/* 183 */	{2, "D=0", 4, 3, "003", 2, 0},
/* 184 */	{28, "LEX", 32, 0, "", 0, 0},
/* 185 */	{1, "?A>C", 7, 3, "806", 2, 0},
/* 186 */	{4, "D0=(4)", 177, 6, "1A000", 3, 4},
/* 187 */	{2, "DCEX", 4, 3, "00F", 2, 0},
/* 188 */	{1, "?C>D", 3, 3, "807", 2, 0},
/* 189 */	{20, "EJECT", 1, 0, "", 0, 0},
/* 190 */	{2, "BSL", 4, 3, "001", 4, 0},
/* 191 */	{13, "DAT1=A", 36, 4, "1510", 0, 0},
/* 192 */	{5, "C=RSTK", 5, 2, "07", 0, 0},
/* 193 */	{5, "CD1EX", 1, 3, "137", 0, 0},
/* 194 */	{5, "AD0EX", 1, 3, "132", 0, 0},
/* 195 */	{1, "?C>B", 7, 3, "805", 2, 0},
/* 196 */	{5, "NOP5", 1, 5, "64000", 0, 0},
/* 197 */	{5, "SHUTDN", 5, 3, "807", 0, 0},
/* 198 */	{4, "CON(6)", 53, 6, "00000", 1, 6},
/* 199 */	{5, "PC=(A)", 1, 4, "808C", 0, 0},
/* 200 */	{1, "?C<=B", 7, 3, "809", 2, 0},
/* 201 */	{3, "A=A&B", 5, 4, "0E00", 0, 0},
/* 202 */	{3, "C=D&C", 5, 4, "0E07", 0, 0},
/* 203 */	{1, "?D#C", 3, 3, "807", 1, 0},
/* 204 */	{2, "CSL", 0, 3, "002", 4, 0},
/* 205 */	{5, "NOP3", 5, 3, "420", 0, 0},
/* 206 */	{5, "AD1XS", 1, 3, "13B", 0, 0},
/* 207 */	{5, "RTN", 5, 2, "01", 0, 0},
/* 208 */	{13, "DAT1=C", 36, 4, "1550", 0, 0},
/* 209 */	{1, "?B#0", 7, 3, "80D", 1, 0},
/* 210 */	{32, "ENTRY", 32, 8, "00000", 0, 0},
/* 211 */	{5, "C+P+1", 1, 3, "809", 0, 0},
/* 212 */	{1, "?A=C", 7, 3, "802", 1, 0},
/* 213 */	{5, "RTNCC", 1, 2, "03", 0, 0},
/* 214 */	{5, "RTNSC", 5, 2, "02", 0, 0},
/* 215 */	{4, "REL(3)", 37, 3, "000", 1, 3},
/* 216 */	{4, "LC(5)", 177, 7, "34000", 3, 5},
/* 217 */	{1, "?C=D", 3, 3, "803", 1, 0},
/* 218 */	{3, "B=A&B", 5, 4, "0E04", 0, 0},
/* 219 */	{3, "D=D&C", 1, 4, "0E03", 0, 0},
/* 220 */	{19, "BSS", 32, 0, "", 0, 0},
/* 221 */	{2, "DSL", 4, 3, "003", 4, 0},
/* 222 */	{4, "CON(1)", 53, 1, "0", 1, 1},
/* 223 */	{44, "IF", 0, 0, "", 0, 0},
/* 224 */	{5, "CD0EX", 1, 3, "136", 0, 0},
/* 225 */	{5, "R2=C", 1, 3, "10A", 0, 0},
/* 226 */	{5, "A=R3", 1, 3, "113", 0, 0},
/* 227 */	{1, "?C=B", 7, 3, "801", 1, 0},
/* 228 */	{5, "ASRB", 5, 3, "81C", 0, 0},
/* 229 */	{4, "LC(3)", 181, 5, "32000", 3, 3},
/* 230 */	{1, "?C=0", 7, 3, "80A", 1, 0},
/* 231 */	{5, "A=R1", 1, 3, "111", 0, 0},
/* 232 */	{5, "R2=A", 5, 3, "102", 0, 0},
/* 233 */	{4, "LC(1)", 177, 3, "300", 3, 1},
/* 234 */	{2, "ASR", 4, 3, "004", 4, 0},
/* 235 */	{5, "AR4EX", 1, 3, "124", 0, 0},
/* 236 */	{5, "CD1XS", 1, 3, "13F", 0, 0},
/* 237 */	{5, "AD0XS", 5, 3, "13A", 0, 0},
/* 238 */	{5, "CLRHST", 5, 3, "82F", 0, 0},
/* 239 */	{4, "GOSUB", 101, 4, "7000", 2, 3},
/* 240 */	{5, "BSRB", 5, 3, "81D", 0, 0},
/* 241 */	{16, "D0=HEX", 0, 4, "1900", 3, 2},
/* 242 */	{38, "CHAIN", 32, 12, "00000", 0, 0},
/* 243 */	{5, "CONFIG", 5, 3, "805", 0, 0},
/* 244 */	{4, "D0=(5)", 177, 7, "1B000", 3, 5},
/* 245 */	{1, "?A<C", 7, 3, "802", 2, 0},
/* 246 */	{1, "?C<D", 3, 3, "803", 2, 0},
/* 247 */	{10, "CPEX", 37, 4, "80F0", 0, 0},
/* 248 */	{3, "B=B&C", 5, 4, "0E01", 0, 0},
/* 249 */	{2, "BSR", 0, 3, "005", 4, 0},
/* 250 */	{2, "A=C+A", 0, 3, "00A", 1, 0},
/* 251 */	{2, "CDEX", 0, 3, "00F", 2, 0},
/* 252 */	{5, "CSRB", 5, 3, "81E", 0, 0},
/* 253 */	{5, "C=R3", 5, 3, "11B", 0, 0},
/* 254 */	{11, "ST=0", 33, 3, "840", 0, 0},
/* 255 */	{1, "?C<B", 7, 3, "801", 2, 0},
/* 256 */	{5, "UNCNFG", 5, 3, "804", 0, 0},
/* 257 */	{5, "C=R1", 1, 3, "119", 0, 0},
/* 258 */	{5, "AR3EX", 1, 3, "123", 0, 0},
/* 259 */	{3, "A=A&C", 1, 4, "0E06", 0, 0},
/* 260 */	{3, "A=C!A", 5, 4, "0E0E", 0, 0},
/* 261 */	{2, "D=D-1", 4, 3, "00F", 1, 0},
/* 262 */	{1, "?C>=D", 7, 3, "80F", 2, 0},
/* 263 */	{2, "CSR", 0, 3, "006", 4, 0},
/* 264 */	{3, "C=B&C", 5, 4, "0E05", 0, 0},
/* 265 */	{5, "CR4EX", 5, 3, "12C", 0, 0},
/* 266 */	{5, "CD0XS", 1, 3, "13E", 0, 0},
/* 267 */	{4, "GOSUBL", 97, 6, "8E000", 3, 4},
/* 268 */	{12, "D0=D0+", 33, 3, "160", 0, 0},
/* 269 */	{2, "D=D+1", 0, 3, "007", 3, 0},
/* 270 */	{4, "D1=(4)", 181, 6, "1E000", 3, 4},
/* 271 */	{1, "?B>C", 7, 3, "801", 2, 0},
/* 272 */	{5, "DSRB", 5, 3, "81F", 0, 0},
/* 273 */	{5, "D0=CS", 1, 3, "13C", 0, 0},
/* 274 */	{5, "INTON", 5, 4, "8080", 0, 0},
/* 275 */	{1, "?A#C", 7, 3, "806", 1, 0},
/* 276 */	{2, "C=C-A", 0, 3, "002", 3, 0},
/* 277 */	{2, "C=C-1", 4, 3, "00E", 1, 0},
/* 278 */	{2, "A=B-A", 0, 3, "00C", 3, 0},
/* 279 */	{1, "?C#D", 7, 3, "807", 1, 0},
/* 280 */	{1, "?B>A", 3, 3, "804", 2, 0},
/* 281 */	{4, "CON(2)", 53, 2, "00", 1, 2},
/* 282 */	{4, "REL(4)", 37, 4, "0000", 1, 4},
/* 283 */	{2, "DSR", 4, 3, "007", 4, 0},
/* 284 */	{5, "D0=AS", 1, 3, "138", 0, 0},
/* 285 */	{5, "MP=0", 1, 3, "828", 0, 0},
/* 286 */	{5, "RSI", 1, 5, "80810", 0, 0},
/* 287 */	{4, "GOVLNG", 53, 7, "8D000", 3, 5},
/* 288 */	{2, "C=C+1", 0, 3, "006", 3, 0},
/* 289 */	{2, "C=C+A", 4, 3, "002", 1, 0},
/* 290 */	{12, "D0=D0-", 33, 3, "180", 0, 0},
/* 291 */	{2, "A=B+A", 0, 3, "000", 1, 0},
/* 292 */	{1, "?C#B", 7, 3, "805", 1, 0},
/* 293 */	{1, "?D>=C", 3, 3, "80B", 2, 0},
/* 294 */	{36, "BIN", 36, 0, "", 0, 0},
/* 295 */	{1, "?C#0", 7, 3, "80E", 1, 0},
/* 296 */	{2, "B=B-1", 0, 3, "00D", 1, 0},
/* 297 */	{2, "B=B-A", 4, 3, "008", 3, 0},
/* 298 */	{5, "AR2EX", 5, 3, "122", 0, 0},
/* 299 */	{5, "CR3EX", 1, 3, "12B", 0, 0},
/* 300 */	{3, "C=C!A", 1, 4, "0E0A", 0, 0},
/* 301 */	{3, "A=B!A", 1, 4, "0E08", 0, 0},
/* 302 */	{3, "C=A&C", 1, 4, "0E02", 0, 0},
/* 303 */	{3, "C=C&D", 5, 4, "0E07", 0, 0},
/* 304 */	{5, "XM=0", 5, 3, "821", 0, 0},
/* 305 */	{4, "GOC", 37, 3, "400", 2, 2},
/* 306 */	{2, "B=B+A", 4, 3, "008", 1, 0},
/* 307 */	{1, "?B=C", 3, 3, "801", 1, 0},
/* 308 */	{2, "B=B+1", 0, 3, "005", 3, 0},
/* 309 */	{1, "?A>=B", 7, 3, "808", 2, 0},
/* 310 */	{2, "B=-B-1", 4, 3, "00D", 4, 0},
/* 311 */	{37, "FORTH", 0, 0, "", 0, 0},
/* 312 */	{2, "A=A-1", 4, 3, "00C", 1, 0},
/* 313 */	{8, "?P=", 35, 3, "890", 0, 0},
/* 314 */	{5, "R3=C", 1, 3, "10B", 0, 0},
/* 315 */	{1, "?B=A", 7, 3, "800", 1, 0},
/* 316 */	{3, "B=B!A", 1, 4, "0E0C", 0, 0},
/* 317 */	{3, "D=C&D", 5, 4, "0E03", 0, 0},
/* 318 */	{2, "A=A+1", 0, 3, "004", 3, 0},
/* 319 */	{18, "LCASC", 32, 2, "30", 3, 2},
/* 320 */	{1, "?D=0", 7, 3, "80B", 1, 0},
/* 321 */	{5, "R3=A", 5, 3, "103", 0, 0},
/* 322 */	{25, "STITLE", 1, 0, "", 0, 0},
/* 323 */	{2, "A=A+A", 4, 3, "004", 1, 0},
/* 324 */	{2, "B=-B", 0, 3, "009", 4, 0},
/* 325 */	{2, "BAEX", 4, 3, "00C", 2, 0},
/* 326 */	{5, "INTOFF", 1, 4, "808F", 0, 0},
/* 327 */	{26, "WORD", 36, 0, "", 0, 0},
/* 328 */	{5, "P=P-1", 5, 2, "0D", 0, 0},
/* 329 */	{2, "B=C-B", 4, 3, "00D", 3, 0},
/* 330 */	{5, "AR1EX", 5, 3, "121", 0, 0},
/* 331 */	{5, "CR2EX", 1, 3, "12A", 0, 0},
/* 332 */	{1, "?B>=A", 7, 3, "80C", 2, 0},
/* 333 */	{1, "?C<=D", 3, 3, "80B", 2, 0},
/* 334 */	{5, "ASLC", 1, 3, "810", 0, 0},
/* 335 */	{16, "D1=HEX", 4, 4, "1D00", 3, 2},
/* 336 */	{2, "D=-D-1", 0, 3, "00F", 4, 0},
/* 337 */	{4, "D1=(5)", 177, 7, "1F000", 3, 5},
/* 338 */	{5, "P=P+1", 5, 2, "0C", 0, 0},
/* 339 */	{2, "B=C+B", 0, 3, "001", 1, 0},
/* 340 */	{1, "?B<C", 7, 3, "805", 2, 0},
/* 341 */	{2, "CAEX", 4, 3, "00E", 2, 0},
/* 342 */	{5, "RSTK=C", 1, 2, "06", 0, 0},
/* 343 */	{2, "C=C-B", 0, 3, "009", 3, 0},
/* 344 */	{1, "?B<A", 7, 3, "800", 2, 0},
/* 345 */	{4, "REL(5)", 37, 5, "00000", 1, 5},
/* 346 */	{4, "CON(3)", 53, 3, "000", 1, 3},
/* 347 */	{5, "BSLC", 5, 3, "811", 0, 0},
/* 348 */	{3, "B=C!B", 1, 4, "0E09", 0, 0},
/* 349 */	{2, "D=-D", 0, 3, "00B", 4, 0},
/* 350 */	{24, "TITLE", 4, 0, "", 0, 0},
/* 351 */	{2, "C=C+B", 0, 3, "009", 1, 0},
/* 352 */	{5, "SETHEX", 1, 2, "04", 0, 0},
/* 353 */	{1, "?D<=C", 7, 3, "80F", 2, 0},
/* 354 */	{5, "CSTEX", 5, 2, "0B", 0, 0},
/* 355 */	{5, "SREQ?", 5, 3, "80E", 0, 0},

	} ;