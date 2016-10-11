#ifdef mdpDevelopmentBuild
   #include "dev_locals.h"
#else
   #include "locals.h"
#endif
#include "model_clothing.h"
#include "math_vector.h"
#include <algorithm>
using std::ifstream;
using std::ios;
using std::string;
using std::swap;
namespace perihelion
{
   // Constructor 
   pMesh::pMesh(void)
   {
      // Initiialize member variables
		mInuse						= false;
		mDataStart					= &mDataStart + 1;
		mDataEnd					= &mDataEnd;
		mDataSize					= ((int)mDataEnd - (int)mDataStart);
		mVertexLump					= NULL;
		mNormalLump					= NULL;
		mTextureLump				= NULL;
		mVertexLumpTranslated	= NULL;
		mNormalLumpTranslated	= NULL;
		mBonePercent				= NULL;
		mPrimitiveLists			= NULL;
		mColorLump					= NULL;
		mVertexCount				= 0;
		mNumPrimitiveLists		= 0;
		mEnableRender				= true;
		mColorList = NULL;	// Colorlist is for devMesh - its temporary, and not saved in the model.
		memset(&mTriangleList, 0, sizeof(mTriangleList));
		memset(&mQuadList, 0, sizeof(mQuadList));

		mlpShader					= NULL;	

		pVector4f_COPY(g_quatIdent, mRotation);
		pVector3f_COPY(g_origin3f, mTranslation);
   }

   pMesh::~pMesh(void)
   {
      gfree(mVertexLump);
      gfree(mNormalLump);
      gfree(mTextureLump);
      gfree(mVertexLumpTranslated);
      gfree(mNormalLumpTranslated);
      gfree(mBonePercent);
      for (int i=0;i<(int)mNumPrimitiveLists;i++)
	    gfree(mPrimitiveLists[i].indexes);
      gfree(mPrimitiveLists);
      gfree(mQuadList.indexes);
      gfree(mTriangleList.indexes);
      gfree(mColorLump);
      gfree(mColorList);
   }

   bool pMesh::LoadOBJ(string fileName)
   {
      ifstream objFile;
      char inputBuffer[SZ_MAX_LENGTH];
      int LOF=0, i=1;
      
      if (mInuse)
      {
//		Sys_Error("pMesh::LoadOBJ(\"%s\") - Mesh already in use!", fileName.c_str());
	    return false;
      }
      
      objFile.open(fileName.c_str());
      if (!objFile.is_open())
      {
//		Sys_Error("pMesh::LoadOBJ(\"%s\"):1 - Could not open file for reading", fileName.c_str());
	    return false;
      }
      
      /*/ Get length of file
      objFile.seekg (0, ios::end);
      LOF = objFile.tellg();
      objFile.seekg (0, ios::beg);
      //*/


      char parms[16][SZ_MAX_LENGTH];
      int numRead;
      int vCount=0, vnCount=0, vtCount=0, tlCount=0, qlCount=0, tfCount=0;	// Array size counts
      
      // Count the number of entries needed for each array
      while (!objFile.eof())
      {
	    objFile.getline(inputBuffer,SZ_MAX_LENGTH, '\n');
	    memset(parms,0,sizeof(parms));
	    numRead = sscanf(inputBuffer, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
		     &parms[0],  &parms[1],  &parms[2],
		     &parms[3],  &parms[4],  &parms[5],
		     &parms[6],  &parms[7],  &parms[8],
		     &parms[9],  &parms[10], &parms[11],
		     &parms[12], &parms[13], &parms[14],
		     &parms[15], &parms[16]);
	    
	    if (!strcmp(parms[0],"v"))			{ vCount++; }
	    elsif (!strcmp(parms[0],"vn"))	{ vnCount++; }
	    elsif (!strcmp(parms[0],"vt"))	{ vtCount++; }
	    elsif (!strcmp(parms[0],"f"))	
	    {
		     if (numRead-1 == 3)			{ tlCount++; }
		     elsif (numRead-1 == 4)	{ qlCount++; }
		     else						{ tfCount++; }
	    }
      };

      // Rewind the file read pointer
      objFile.seekg (0, ios::beg);
      objFile.clear();
      objFile.close();
      objFile.open(fileName.c_str());
      

      // If we have no verts, or fewer normals than verts, we can't read this file
      if (!vCount || (vnCount && vnCount < vCount))
      {
//		Sys_Error("pMesh::LoadOBJ(\"%s\") - Bad number of verts/normals", fileName);
	    return false;
      }
      // Allocate the memory for a temporary vertex array - we may have to shift these
      // verts around depending on how the normals are done.
      pVector3f_t	*lTempVertexLump = (pVector3f_t*) gmalloc( vCount * 3 * sizeof(float));
      
      // If there are more vert normals than verts, resize vert list to have 1vert per normal
      if (vnCount > vCount) vCount = vnCount;
      mVertexCount			= vCount;
      mVertexLump				= (pVertex*)	gmalloc( (vCount) * 3 * sizeof(float));
      mVertexLumpTranslated	= (pVertex*)	gmalloc( (vCount) * 3 * sizeof(float));
      mNormalLumpTranslated	= (pVertex*)	gmalloc( (vnCount) * 3 * sizeof(float));
      mNormalLump				= (pVector3f*)	gmalloc( (vnCount) * 3 * sizeof(float));
      mBonePercent			= (unsigned char*) gmalloc((vCount) * sizeof(unsigned char));
//	mTextureLump			= (pVector3f*)	gmalloc(vtCount * 3 * sizeof(float));	// ignoring textures for now
      
      mNumPrimitiveLists = tfCount;
      mPrimitiveLists = (glList_t*)		gmalloc(tfCount * sizeof(glList_t));
      memset(mPrimitiveLists,0,tfCount * sizeof(glList_t));

      mTriangleList.indexes	= (GLuint*) gmalloc(tlCount * 3 * sizeof(GLuint));
      mQuadList.indexes		= (GLuint*) gmalloc(qlCount * 4 * sizeof(GLuint));


      // Actually begin loading in the vertex/face data
      int vIndex=0,vnIndex=0,vtIndex=0;
      int currentPrimitiveList=0;

      while (!objFile.eof())
      {
	    objFile.getline(inputBuffer,SZ_MAX_LENGTH, '\n');
	    memset(parms,0,sizeof(parms));
	    numRead = sscanf(inputBuffer, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
		     &parms[0],  &parms[1],  &parms[2],
		     &parms[3],  &parms[4],  &parms[5],
		     &parms[6],  &parms[7],  &parms[8],
		     &parms[9],  &parms[10], &parms[11],
		     &parms[12], &parms[13], &parms[14],
		     &parms[15], &parms[16]);

	    if (!strcmp(parms[0],"v")) 
	    {
		     lTempVertexLump[vIndex].x = float(atof(parms[1]));
		     lTempVertexLump[vIndex].y = float(atof(parms[3]));		// Perihelion uses Z as up
		     lTempVertexLump[vIndex].z = float(atof(parms[2]));
		     vIndex++;
	    }
	    elsif (!strcmp(parms[0],"vn")) 
	    {
		     mNormalLump[vnIndex][0] = float(atof(parms[1]));
		     mNormalLump[vnIndex][1] = float(atof(parms[3]));// Perihelion uses Z as up
		     mNormalLump[vnIndex][2] = float(atof(parms[2]));
		     vnIndex++;
	    }
	    elsif (!strcmp(parms[0],"vt")) 
	    {
/*			VertTextureArray[vtIndex].x = atof(parms[1]);
		     VertTextureArray[vtIndex].y = atof(parms[2]);
		     VertTextureArray[vtIndex].z = atof(parms[3]);*/
		     vtIndex++;
	    }
	    elsif (!strcmp(parms[0],"f")) 
	    {	
		     int vertInfo[16][3];	//  'VERT/TEXTURE/NORMAL' - max 16 trios
		     // For each vert in this face
		     for (i=1; (parms[i][0] != '\0' && i < 16);i++)
		     {
			      char* numOffset[3];
			      // Find the beginning of each number in vertex trio
			      numOffset[0] = parms[i];
			      if ((int)numOffset[0] > 1) numOffset[1] = strchr(numOffset[0],'/') + 1;
			      if ((int)numOffset[1] > 1) numOffset[2] = strchr(numOffset[1],'/') + 1;
			      
			      // Find the VALUE of each number in the vertex trio
			      
			      vertInfo[i][0] = int(atof(numOffset[0])) - 1;
			      
			      if (int(numOffset[1]) > 1)
				    vertInfo[i][1] = int(atof(numOffset[1])) - 1;
			      else
				    vertInfo[i][1] = -1;

			      if (int(numOffset[1]) > 1)
				    vertInfo[i][2] = int(atof(numOffset[2])) - 1;
			      else
				    vertInfo[i][2] = -1;

			      // Upload vert coordinates into mesh member lump using new numbering
			      // use vertNormal numbering for indexes if there IS normal info . . .
			      if ( vertInfo[i][2] >= 0)
			      {
				    if (vertInfo[i][2] == 7180)
				    {
					     int bob=3;
				    }
				    // mLumps use 0->N-1 numbering.  Convert 1->N numbering, using vNormal index nums
				    this->mVertexLump[ vertInfo[i][2] ][0] = lTempVertexLump[ vertInfo[i][0] ].x;
				    this->mVertexLump[ vertInfo[i][2] ][1] = lTempVertexLump[ vertInfo[i][0] ].y;
				    this->mVertexLump[ vertInfo[i][2] ][2] = lTempVertexLump[ vertInfo[i][0] ].z;
				    // Default each vertex to 0% affinity to parent bone, 100% owner bone
				    this->mBonePercent[vertInfo[i][2]] = 100;
			      }
			      // . . . otherwise use straight vertex numbering
			      else
			      {
				    // mLumps use 0->N-1 numbering.  Convert 1->N numbering, using Vertex index nums
				    this->mVertexLump[ vertInfo[i][0] ][0] = lTempVertexLump[vertInfo[i][0] ].x;
				    this->mVertexLump[ vertInfo[i][0] ][1] = lTempVertexLump[vertInfo[i][0] ].y;
				    this->mVertexLump[ vertInfo[i][0] ][2] = lTempVertexLump[vertInfo[i][0] ].z;
				    // Default each vertex to 0% affinity to parent bone, 100% owner bone
				    this->mBonePercent[vertInfo[i][0]] = 100;
			      }
		     };
		     int whichFaceType;
		     glList_t *listPointer;

		     switch(i-1) 
		     {
			      case 3:
				    whichFaceType = triList;
				    listPointer = &mTriangleList;
				    break;
			      case 4:
				    whichFaceType = quadList;
				    listPointer = &mQuadList;
				    break;
			      default:
				    whichFaceType = triFans;
				    listPointer = &mPrimitiveLists[currentPrimitiveList];
				    listPointer->indexes = (GLuint*)gmalloc(sizeof(GLuint) * (i-1));
				    listPointer->type = whichFaceType;
				    currentPrimitiveList ++;
				    break;
		     };

		     for (int j=1;j<i;j++)
		     {
			      // If we're using the numbering for normals... 
			   //   if ( vertInfo[i][2] != 0)
				    listPointer->indexes[ listPointer->length + j-1 ] = vertInfo[i-j][2];
			   //   else 
			   //    listPointer->indexes[ listPointer->length + j-1 ] = vertInfo[i-j][0];
		     }
		     // Lengthen the current list by the number of verts on this face [the -1 is because vertInfo starts at index 1]
		     listPointer->length += i-1;

	    } // End loop through faces

      }; // End loop through .obj file lines
      mInuse = true;
      gfree(lTempVertexLump);
      UpdateVertexColors(0.35f, 0.60f, 1.00f);
//	Con_Printf("Loaded Mesh from \"%s\": %i verts, %i vert normals, %i texture coordinates",fileName.c_str(), vIndex, vnIndex, vtIndex);
      return true;
   }

   // Accesors
   void pMesh::Update(void)
   {
	    memcpy(mVertexLumpTranslated, mVertexLump, mVertexCount * sizeof(float) * 3);	// Copy identity frame to translated vert list
	    memcpy(mNormalLumpTranslated, mNormalLump, mVertexCount * sizeof(float) * 3);	// Copy identity frame to translated vert list
   }

   void pMesh::UpdateVertexColors(float r, float g, float b)
   {
	    int i;
		// why would you delete and reallocate every single time?
	    //if (mColorLump != NULL)
		//  gfree(mColorLump);

		if(mColorLump == NULL)
	    mColorLump = (pVector3f*) gmalloc(sizeof(float)*3 * mVertexCount);

	    for (i=0; i<(int)mVertexCount;i++)
	    {
		  // Uncomment this code for some fun : ) -LCW
		  mColorLump[i][0] = r;// + i/200.0 - int(g + i/200.0); + i/float(mVertexCount) - int(r + i/float(mVertexCount));
		  mColorLump[i][1] = g;// + i/200.0 - int(g + i/200.0);
		  mColorLump[i][2] = b;// + i/30.0 - int(b + i/30.0);
		  //mColorLump[i][3] = 1;
	    }
   
	    
   }

   void pMesh::UpdateVertexShirtColors(float r, float g, float b, bool male, std::vector<int> & maleShirt, std::vector<int> & femaleShirt)
   {

		int i;
	    int numberVertShirt;

	    if (male) {
			numberVertShirt = maleShirt.size();
	    } else {
			numberVertShirt = femaleShirt.size();
	    }

	    for (i = 0; i < numberVertShirt; i++) {
			if (male) {
				mColorLump[maleShirt[i]][0] = r;
				mColorLump[maleShirt[i]][1] = g;
				mColorLump[maleShirt[i]][2] = b;
			} else {
				mColorLump[femaleShirt[i]][0] = r;
				mColorLump[femaleShirt[i]][1] = g;
				mColorLump[femaleShirt[i]][2] = b;
			}
	    }


		// The following is legacy code
		/*
	    int i;
	    int numberVertShirt;
	    if (male)
	    {
		  numberVertShirt = MALE_NUM_VERT_SHIRT;
	    } else {
		  numberVertShirt = FEMALE_NUM_VERT_SHIRT;
	    }
	    for (i = 0; i < numberVertShirt; i++)
	    {
		  if (male)
		  {
			   mColorLump[maleShirtVertList[i]][0] = r;
			   mColorLump[maleShirtVertList[i]][1] = g;
			   mColorLump[maleShirtVertList[i]][2] = b;
			   //mColorLump[maleShirtVertList[i]][3] = 1;
		  } else {
			   mColorLump[femaleShirtVertList[i]][0] = r;
			   mColorLump[femaleShirtVertList[i]][1] = g;
			   mColorLump[femaleShirtVertList[i]][2] = b;
			   //mColorLump[femaleShirtVertList[i]][3] = 1;
		  }
	    }
	    return;
		*/
   }

   void pMesh::UpdateVertexPantColors(float r, float g, float b, bool male, std::vector<int> & malePant, std::vector<int> & femalePant)
   {
	    int i;
	    int numberVertPants;
	    if (male) {
			numberVertPants = malePant.size();
	    } else {
			numberVertPants = femalePant.size();
	    }
	    for (i = 0; i < numberVertPants; i++) {
			if (male) {
				mColorLump[malePant[i]][0] = r;
				mColorLump[malePant[i]][1] = g;
				mColorLump[malePant[i]][2] = b;
			} else {
				mColorLump[femalePant[i]][0] = r;
				mColorLump[femalePant[i]][1] = g;
				mColorLump[femalePant[i]][2] = b;
			}
	    }


		// The following is legacy code
		/*
	    int i;
	    int numberVertPants;
	    if (male)
	    {
		  numberVertPants = MALE_NUM_VERT_PANTS;
	    } else {
		  numberVertPants = FEMALE_NUM_VERT_PANTS;
	    }
	    for (i = 0; i < numberVertPants; i++)
	    {
		  if (male)
		  {
			   mColorLump[malePantVertList[i]][0] = r;
			   mColorLump[malePantVertList[i]][1] = g;
			   mColorLump[malePantVertList[i]][2] = b;
			   //mColorLump[malePantVertList[i]][3] = 1;
		  } else {
			   mColorLump[femalePantVertList[i]][0] = r;
			   mColorLump[femalePantVertList[i]][1] = g;
			   mColorLump[femalePantVertList[i]][2] = b;
			   //mColorLump[femalePantVertList[i]][3] = 1;
		  }
	    }
		*/
   }

   void pMesh::DrawVerts(unsigned int lCount, unsigned int* lIndexes, GLenum lDrawMethod) const
   {
	    glEnableClientState ( GL_VERTEX_ARRAY );
	    glVertexPointer		( 3, GL_FLOAT, 0, mVertexLump );
	    glDrawElements(lDrawMethod,	lCount,	GL_UNSIGNED_INT,	lIndexes);
   }

   void pMesh::Draw(void)
   {
	    if (!mEnableRender) 
			return;

	    glEnableClientState ( GL_VERTEX_ARRAY );
	    glVertexPointer		( 3, GL_FLOAT, 0, mVertexLump );
	    glVertexPointer		( 3, GL_FLOAT, 0, mVertexLumpTranslated );
	    glEnableClientState ( GL_NORMAL_ARRAY );
	    glNormalPointer		( GL_FLOAT, 0, mNormalLump );
	    glNormalPointer		( GL_FLOAT, 0, mNormalLumpTranslated );
	    
	    if (mColorLump)
	    {
		  glEnableClientState ( GL_COLOR_ARRAY );
		  glColorPointer		( 3, GL_FLOAT, 0, mColorLump );	
	    }
	    /*
	    glList_t *current;
	    for (int i=0;i<(int)this->mNumPrimitiveLists;i++)
	    {
		  current = &this->mPrimitiveLists[i];
		  if (current->length != 0)
			   glDrawElements(current->type + 4,	current->length-1,
							    GL_UNSIGNED_INT,	current->indexes);
	    }
	    if (mTriangleList.length)
		  glDrawElements(GL_TRIANGLES,	mTriangleList.length,	GL_UNSIGNED_INT,	mTriangleList.indexes);
		  */
	    if (mQuadList.length)
		  glDrawElements(GL_QUADS,		mQuadList.length,		GL_UNSIGNED_INT,	mQuadList.indexes);

	    if (mColorLump)
		  glDisableClientState ( GL_COLOR_ARRAY );
	    glDisableClientState ( GL_VERTEX_ARRAY );
	    glDisableClientState ( GL_NORMAL_ARRAY );

   }	

   void pMesh::RotateVertListByQuats(float iBoneScale, float iWeightScale, const pVector3f iJointOrigin, const pVector3f iDistalOrigin, const pVector3f iTJointOrigin, const pQuat iqParent, const pQuat iqChild, const pQuat iqRef, const int iCount, const GLuint *iIndexes)
   {

	    pVector3f	ivToDistal,vBone;
	    pVector3f	vPerpendicular, vParallel;

	    pQuat		lScaledChildRot, lResultQuat;
	    int			i;

	    float boneLength;
	    pVector3f_SUBTRACT(iDistalOrigin, iJointOrigin, vBone);
	    boneLength = pVectorLength(vBone) / iBoneScale;	// distal already has bonescaling applied - remove it so we can scale the verts correctly
	    pVectorNormalize(vBone, vBone);


	    for (i=0; i<iCount; i++)
	    {
		  int weight = mBonePercent[iIndexes[i]];
		  if (weight == 0)
		  {
			   pVector4f_COPY(g_quatIdent, lScaledChildRot);
//				pVector4f_COPY(iqRef, lScaledChildRot);
		  }
		  else
		  {
			   // Scale the local rotation to 'BonePercent' for that vert.
//				pQuatMultiply		(iqChild			, iqRef		, lScaledChildRot	);
//				pVector4f_COPY(iqRef, lScaledChildRot);
			   pVector4f_COPY		(iqChild			, lScaledChildRot						);
			   lScaledChildRot[3] *= 100.0f / weight;
	    //		if (mVertexLump[iIndexes[i]][2] < 9)
	    //			lScaledChildRot[3] /= (mVertexLump[iIndexes[i]][2] - 8.0f) / 0.5;
		  }

		  double weightedWeightScale = 1.0;
		  if (iWeightScale > 1.0)
			   weightedWeightScale = (iWeightScale - 1.0) * (weight / 100.0) + 1.0;
		  if (iWeightScale < 1.0)
			   weightedWeightScale = 1.0 - (1.0 - iWeightScale) * (weight / 100.0);

//			iBoneScale = 1.0;
		  double weightedBoneScale = 1.0; // = (iBoneScale * 100.0 / weight + 1.0) / 2.0;
		  if (iBoneScale > 1.0)
			   weightedBoneScale = (iBoneScale - 1.0) * (weight / 100.0) + 1.0;
		  if (iBoneScale < 1.0)
			   weightedBoneScale = 1.0 - (1.0 - iBoneScale) * (weight / 100.0);

		  pVectorNormalize4f	(lScaledChildRot	, lScaledChildRot						);

//			pQuatMultiply		(iqRef						, iqParent		, lResultQuat	);
//			pVectorNormalize4f	(lResultQuat				, lResultQuat					);
//			pQuatMultiply		(lScaledChildRot			, lResultQuat	, lResultQuat	);
//			pVectorNormalize4f	(lResultQuat				, lResultQuat					);

		  pQuatMultiply		(lScaledChildRot			, iqParent		, lResultQuat	);

		  pVertex reOriginatedVert;
		  pQuatRotateVector(mRotation, mVertexLump[iIndexes[i]], reOriginatedVert);
		  pVector3f_ADD(reOriginatedVert, mTranslation, reOriginatedVert);
		  pVector3f_SUBTRACT	(reOriginatedVert, iJointOrigin	, ivToDistal	);

		  float distanceAlongBone = pVector3f_DOT(vBone, ivToDistal);		
		  pVector3f_SCALE(vBone, distanceAlongBone, vParallel);
		  pVector3f_SUBTRACT(ivToDistal, vParallel, vPerpendicular);

		  pVector3f_SCALE(vParallel, float(weightedBoneScale), vParallel);
		  pVector3f_SCALE(vPerpendicular, float(weightedWeightScale), vPerpendicular);

		  pVector3f_ADD		(vParallel, vPerpendicular, ivToDistal);
		  pQuatRotateVector	(lResultQuat				, ivToDistal	, ivToDistal	);
		  pVector3f_ADD		(ivToDistal					, iTJointOrigin	, mVertexLumpTranslated[iIndexes[i]]);

		  // Rotate the NORMALS for this index as well
		  pQuatRotateVector	(lResultQuat				, mNormalLump[iIndexes[i]], mNormalLumpTranslated[iIndexes[i]]);
	    }

   }
	void pMesh::ScaleMesh(double x, double y, double z)
	{
		bool invert = false;
		if (x*y*z < 0)
			invert = true;

		for (unsigned int i=0;i<mVertexCount;i++)
		{
			mVertexLump[i][0] *= x;
			mVertexLump[i][1] *= y;
			mVertexLump[i][2] *= z;
			mNormalLump[i][0] *= x;
			mNormalLump[i][1] *= y;
			mNormalLump[i][2] *= z;
			pVectorNormalize(mNormalLump[i], mNormalLump[i]);
		}
		// If we scaled our mesh inside out, we need to reverse the winding of the primitives we draw.
		if (invert)
		for (int j=0;j<mQuadList.length;)
		{
			swap( mQuadList.indexes[j], mQuadList.indexes[j+3]); 
			swap( mQuadList.indexes[j+1], mQuadList.indexes[j+2]);
			j += 4;
		}

	}

   size_t pMesh::size(void)
   {
	    int lSerialLength = 0;
	    int i;

	    lSerialLength += mDataSize;

	    if (mVertexLump != NULL)
		  lSerialLength += mVertexCount * sizeof(pVertex);			// For mVertexLump
	    if (mNormalLump != NULL)
		  lSerialLength += mVertexCount * sizeof(pVector3f);			// For mNormalLump
	    if (mTextureLump != NULL)
		  lSerialLength += mVertexCount * sizeof(pVector3f);			// For mTextureLump
	    if (mBonePercent != NULL)
		  lSerialLength += mVertexCount * sizeof(unsigned char);		// For mTextureLump
	    for (i=0; i<(int)mNumPrimitiveLists; i++)
	    {
		  lSerialLength += sizeof(glList_t);
		  lSerialLength += mPrimitiveLists[i].length * sizeof(GLuint);
	    }
	    lSerialLength += mQuadList.length		* sizeof (GLuint);
	    lSerialLength += mTriangleList.length	* sizeof (GLuint);
	    lSerialLength += 256;											// for shader name
	    if (mColorLump != NULL)
		  lSerialLength += mVertexCount * sizeof(pVector3f);			// For mTextureLump
	    return lSerialLength;
   }

   #define LUMP_TO_FILE(var, type, count) {							\
	    memcpy(oSerialLump + lOffset, var, count * sizeof(type));		\
	    lOffset += count * sizeof(type);								\
	    }

   #define FILE_TO_LUMP(var, type, count) {										\
	    const size_t lumpSize = count * sizeof(type);								\
	    GLuint* lpVertArrayLocation = (GLuint*)((unsigned char*)iSerialLump + lOffset); \
	    var = (type*) gmalloc(lumpSize);											\
	    memcpy(var, lpVertArrayLocation, lumpSize);									\
	    lOffset += lumpSize;														\
	    }

   int	pMesh::Serialize (unsigned char* &oSerialLump)
   {
	    int i;
	    int lOffset = 0;
	    int lSerialLength = this->size();


	    oSerialLump = (unsigned char*)gmalloc(lSerialLength * sizeof(unsigned char));
	    memset(oSerialLump, 0, lSerialLength * sizeof(unsigned char));


	    // Save the member variables
	    memcpy(oSerialLump, mDataStart, mDataSize);
	    lOffset += mDataSize;

	    if (mVertexLump)
		  LUMP_TO_FILE(mVertexLump, pVertex, mVertexCount);
	    if (mNormalLump)
		  LUMP_TO_FILE(mNormalLump, pVector3f, mVertexCount);
	    if (mTextureLump)
		  LUMP_TO_FILE(mTextureLump, pVector3f, mVertexCount);
	    if (mBonePercent)
		  LUMP_TO_FILE(mBonePercent, unsigned char, mVertexCount);

	    LUMP_TO_FILE(mPrimitiveLists, glList_t, mNumPrimitiveLists);

	    for (i=0; i<(int)mNumPrimitiveLists; i++)
		  LUMP_TO_FILE(mPrimitiveLists[i].indexes, GLuint, mPrimitiveLists[i].length);

	    LUMP_TO_FILE(mQuadList.indexes, GLuint, mQuadList.length);
	    LUMP_TO_FILE(mTriangleList.indexes, GLuint, mTriangleList.length);

	    if (mlpShader != NULL)
   //		strcpy(oSerialLump + lOffset, mlpShader->Name);
		  memcpy((char*)oSerialLump + lOffset, "NO NAME SHADER",256);
	    lOffset += 256;											// for shader name

	    if (mColorLump != NULL)
		  LUMP_TO_FILE(mColorLump, pVector3f, mVertexCount);

	    return lOffset;
   }



   bool pMesh::Deserialize (unsigned char* &iSerialLump)
   {
	    int i;
	    int lSerialLength = 0;
	    int lOffset = 0;

	    // Save the member variables
	    memcpy(mDataStart, iSerialLump, mDataSize);
	    lOffset += mDataSize;

	    /* If there is supposed to be data in this lump (aka, a pointer was 
	    * saved) that pointer will be garbage, as the structure of data in
	    * memory this time around isnt always the exact same.  So, we need
	    * to allocate the memory for the data lumps, set the pointers to
	    * these allocated memory chunks, and put the correct data in these
	    * memory chunks.
	    */

	    if (mVertexLump)
		  FILE_TO_LUMP(mVertexLump, pVertex, mVertexCount);
	    if (mNormalLump)
		  FILE_TO_LUMP(mNormalLump, pVector3f, mVertexCount);
	    if (mTextureLump)
		  FILE_TO_LUMP(mTextureLump, pVector3f, mVertexCount);
	    if (mBonePercent)
		  FILE_TO_LUMP(mBonePercent, unsigned char, mVertexCount);

	    // Create memory for dynamic data locations
	    mVertexLumpTranslated = (pVertex*) gmalloc(mVertexCount * sizeof(pVertex));
	    mNormalLumpTranslated = (pVertex*) gmalloc(mVertexCount * sizeof(pVertex));

	    // Load in primitve lists
	    FILE_TO_LUMP(mPrimitiveLists, glList_t, mNumPrimitiveLists);
	    for (i=0; i<(int)mNumPrimitiveLists; i++)
		  FILE_TO_LUMP(mPrimitiveLists[i].indexes, GLuint, mPrimitiveLists[i].length);

	    // Load in list of every quad used in model
	    FILE_TO_LUMP(mQuadList.indexes, GLuint, mQuadList.length);
	    // Load in list of every triangle used in model
	    FILE_TO_LUMP(mTriangleList.indexes, GLuint, mTriangleList.length);


	    {
		  GLuint* lpVertArrayLocation = (GLuint*)((unsigned char*)iSerialLump + lOffset);
		  char lTmpString[257];
	// */8	  
		  memset(lTmpString,0,sizeof(lTmpString));
		  memcpy(lTmpString,(const char*)lpVertArrayLocation,256);

   //		this->mlpShader = LoadShader(tmpString);

		  lOffset += 256;											// for shader name
	    }

	    if (mColorLump != NULL)
		  FILE_TO_LUMP(mColorLump, pVector3f, mVertexCount);

		mColorList = NULL;	// Colorlist is for devMesh - its temporary, and not saved in the model.


	    return true;
   }
}