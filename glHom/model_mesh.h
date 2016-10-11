#ifndef __MODEL_MESH_H__
#define __MODEL_MESH_H__
#include <vector>


namespace perihelion
{

	// =======================================================================================
	// The mesh class is attached to a vertex lump and a 'shader'.  The shader lists how the
	// mesh is to be drawn.  The vertex lump contains at least all the verts needed for the 
	// primitves in the current mesh.  Each mesh can contain multiple lists of primitves
	// which will be rendered in arbitrary order [meaning depth checking must be on to ensure
	// proper results]
	class pMesh
	{
	public:
		// Constructor
		pMesh(void);

		// Destructor
		~pMesh(void);

		// Constant
		virtual void	Draw(void);						// Draws entire model
		void	DrawVerts(unsigned int lCount, unsigned int* lIndexes, GLenum lDrawMethod) const;

		// Mutators
		//inline	void	SetShader	(pShader* newShader) { mlpShader = newShader; } 
		void	Update		(void);	
		bool	LoadOBJ(std::string fileName);
		virtual	void	UpdateVertexColors(float r, float g, float b);
		virtual void	UpdateVertexShirtColors(float r, float g, float b, bool male, std::vector<int> & maleShirt, std::vector<int> & femaleShirt);
		virtual void	UpdateVertexPantColors(float r, float g, float b, bool male, std::vector<int> & malePant, std::vector<int> & femalePant);
		virtual void	SetSelectedBoneVertlist(int vertCount, int *indexes) {;}
		void RotateVertListByQuats(float iBoneScale, float iWeightScale, const pVector3f iJointOrigin, const pVector3f iDistalOrigin, const pVector3f iTJointOrigin, const pQuat iqParent, const pQuat iqChild, const pQuat iqRef, const int iCount, const GLuint *iIndexes);
		//	int			Select(const pVertex lOutline[4], int lSelectType) const;
		void SetTranslation(pVertex translation) { pVector3f_COPY(translation, mTranslation); }
		void SetRotation   (pQuat rotation       ) { pVector4f_COPY(rotation, mRotation);       }
		// Accessors
		inline	int		GetLength(void) const { return mVertexCount; }
		int		Serialize		(unsigned char* &oSerialLump);
		bool	Deserialize		(unsigned char* &oSerialLump);
		void	Enable(bool enable) { mEnableRender = enable; }

		size_t	size			(void);
		bool Inuse() { return mInuse; }
		void	ScaleMesh(double x=1.0, double y=1.0, double z=1.0);
		int GetChecksum() const;
		bool Validate() const { return (GetChecksum() == mChecksum); }
	
	protected:

		size_t		mDataSize;			
		void*		mDataStart;				
		GLuint		mVertexCount;
		pVertex		*mVertexLump;			// Mesh vertexes	[one giant vertex lump]
		pVector3f	*mNormalLump;			// Vert normals
		pVector3f	*mTextureLump;			// Texture coordinates
		pVertex		*mNormalLumpTranslated;	// Post Translaton Mesh vertexes
		pVertex		*mVertexLumpTranslated;	// Post Translaton Mesh vertexes
		pVector3f	*mColorLump;			// Per-vertex color data
		unsigned char *mBonePercent;		// PCent Parent Bone Affinity
		

		glList_t	*mPrimitiveLists;		
		GLuint		mNumPrimitiveLists;		

		glList_t	mQuadList;				
		glList_t	mTriangleList;			
		

		pShader		*mlpShader;				
		bool		mInuse;
		pVector3f		*mColorList;		// color list for verts

		// NOTHING AFTER THIS POINT WILL BE SAVED
		void*		mDataEnd;				
		pVector3f	mTranslation;
		pQuat		mRotation;
		bool		mEnableRender;
		int         mChecksum;
	};
}
#endif