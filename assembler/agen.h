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

/******************************************************************************

			       AREUH ASSEMBLER

		   GLOBAL DECLARATIONS FOR CODE GENERATION


******************************************************************************/

#define F_LSET 0x01
#define F_RETY 0x02
#define F_TREE 0x04
#define F_GOYS 0x08
#define F_ABSL 0x10
#define F_EXPR 0x20
#define F_GSUB 0x40
#define F_PART 0x80

#define fs_A 9
#define fs_B 7

#define ar_C 1
#define ar_D 2
#define ar_E 3
#define ar_F 4

extern char hex () ;
extern int dec () ;
