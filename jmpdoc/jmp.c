#define OUI 1
#define NON 0
#define JMPVIDE {0,0,0,0,0,0,0,0}

struct jmpcode
{
    char *jc_text [4] ;		/* les 4 lignes au max. d'un goto */
    int jc_long ;		/* longueur en quartets */
    saddr jc_min, jc_max ;
    int jc_invert ;		/* OUI s'il faut inverser le test, NON sinon */
} ;

struct jmpcode jmptable [7][6] =
{

/* BETC */
    {   {"GOC %s", 0, 0, 0, 3, -127, 128, NON},
	{"GOTO %s", 0, 0, 0, 4, -2047, 2048, NON},
	{"GOLONG %s", 0, 0, 0, 6, -32766, 32768, NON},
	JMPVIDE,
	JMPVIDE,
	JMPVIDE},

/* BETNC */
    {	{"GONC %s", 0, 0, 0, 3, -127, 128, NON},
	{"GOTO %s", 0, 0, 0, 4, -2047, 2048, NON},
	{"GOLONG %s", 0, 0, 0, 6, -32766, 32768, NON},
	JMPVIDE,
	JMPVIDE,
	JMPVIDE},

/* JMP */
    {	{"GOTO %s", 0, 0, 0,  4, -2047, 2048, NON},
	{"GOLONG %s", 0, 0, 0, 6, -32766, 32768, NON},
	JMPVIDE,
	JMPVIDE,
	JMPVIDE,
	JMPVIDE},

/* JMPC */
    {	{"GOC %s", 0, 0, 0, 3, -127, 128, NON},
	{"GONC *+7", "GOTO %s", 0, 0, 7, -2044, 2051, NON},
	{"GONC *+9", "GOLONG %s", 0, 0, 9, -32763, 32772, NON},
	JMPVIDE,
	JMPVIDE,
	JMPVIDE},

/* JMPNC */
    {	{"GONC %s", 0, 0, 0, 3, -127, 128, NON},
	{"GOC *+7", "GOTO %s", 0, 0, 7, -2044, 2051, NON},
	{"GOC *+9", "GOLONG %s", 0, 0, 9, -32763, 32772, NON},
	JMPVIDE,
	JMPVIDE,
	JMPVIDE},

/* JMPOUI */
    {	{"GOYES %s", 0, 0, 0, 2, -128, 127, NON},
    	{"GOYES *+6", "GOTO %s", 0, 0, 6, -2045, 2050, OUI},
	{"GOYES *+8", "GOLONG %s", 0, 0, 8, -32764, 32771, OUI},
	{"GOYES *+5", "GONC *+6", "GOTO %s", 0, 9, -2042, 2053, NON},
	{"GOYES *+5", "GONC *+8", "GOLONG %s", 0, 11, -32759, 32776, NON},
	JMPVIDE},

/* JSR */
    {	{"GOSUB %s", 0, 0, 0, 4, -2044, 2051, NON},
    	{"GOSUBL %s", 0, 0, 0, 6, -32762, 32773, NON},
	JMPVIDE,
	JMPVIDE,
	JMPVIDE,
	JMPVIDE},
} ;
