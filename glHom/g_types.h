#ifndef __G_TYPES_H__
#define __G_TYPES_H__
namespace perihelion
{
	#define elsif else if
	//
	// String MAX length for normal string arrays
	#define SZ_MAX_LENGTH 4096

	#define NOOP	{int bob=3;}

	//
	// Setting indicating value is un-initialized!
	// Used for debugging perposes, so uninitalized
	// values can be tracked down easier
	#define DEFAULT_UNSET	-1
	#define RET_ERR			-1

	#define byte BYTE

	////#define uint unsigned int

	#define PITCH	0
	#define YAW		1
	#define ROLL	2

	typedef GLfloat pVertex[3];

	typedef struct tagfRECT
	{
		float    left;
		float    top;
		float    right;
		float    bottom;
	} fRECT, *fLPRECT;

	typedef struct tagfPOINT
	{
		float	x;
		float	y;
	} fPOINT, *fPPOINT, NEAR *fNPPOINT, FAR *fLPPOINT;
}
#endif