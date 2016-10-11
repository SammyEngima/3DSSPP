#ifdef mdpDevelopmentBuild
	#include "dev_locals.h"
#else
	#include "locals.h"
#endif

using std::iostream;	using std::endl;
using std::string;

namespace perihelion
{

	pBone::pBone(void) 
	{
		Initialize();
	}

	pBone::pBone(pVertex iDistal) 
	{
		Initialize();
		pVector3f_COPY(iDistal  , mDistal);	
	}

	pBone::pBone(pVertex iDistal, pVector3f iRefVect) 
	{
		Initialize();
		pVector3f_COPY(iDistal  , mDistal);	
	}

	void pBone::Initialize(void)
	{
		mDataStart				= &mDataStart + 1;
		mDataSize				= sizeof(*this) - ((int)mDataStart - (int)this);
		pVector3f_COPY(g_origin3f, mProximal);
		pVector3f_COPY(g_origin3f, mDistal);
		pVector3f_COPY(g_origin3f, mBoneVector);
		pVector4f_COPY(g_quatIdent, mReferenceRotation);
		pVector4f_COPY(g_quatIdent, mLocalRotation);
		pVector4f_COPY(g_quatIdent, mGlobalRotation);
		pVector4f_COPY(g_quatIdent, mParentGlobalRotation);
		mChildren		= NULL;
		mChildrenCount	= 0;
		mInuse			= false;
		mSelected		= false;
		mParent			= NULL;
		memset(mAttachedVerts,0,sizeof(mAttachedVerts));
		memset(mAttachedVertCount,0,sizeof(mAttachedVertCount));

		mBoneName = "";
		mWeight = 1.0f;
		mLength = 1.0f;
		mReferenceWeight = 1.0f;
		mReferenceLength = 1.0f;
	}
	void pBone::DumpInfo(iostream &outFile)
	{
		
		#ifdef mdpDevelopmentBuild
			outFile << mBoneName << endl;
			outFile << mWeight << "\t";
			outFile << mLength << "\t";
			outFile << mReferenceWeight << "\t";
			outFile << mReferenceLength << "\t";
		#else
			outFile << mBoneName << endl;
			outFile << mWeight << "\t";
			outFile << perihelion::pVectorLength(GetVector()) / 10.0f << endl;
			outFile << mReferenceWeight << "\t";
			outFile << perihelion::pVectorLength(GetVector()) / 10.0f << endl;
		#endif
	
	}
	void pBone::LoadInfo(iostream &inFile, bool updateName)
	{
		char tmpString[1024];
		memset(tmpString,0,sizeof(tmpString));
		inFile.getline(tmpString, 1023);
		if (updateName)
			mBoneName = tmpString;
		else if (mBoneName != tmpString)
		{
			throw string("Unexpected bone name");
			return;
		}

		inFile >> mWeight;
		inFile >> mLength;
		inFile >> mReferenceWeight;
		inFile >> mReferenceLength;
		inFile.ignore(2, '\n');
	}
	void pBone::LoadInfo(const boneInfo& bI, bool updateName)
	{
		mWeight = bI.weight;
		mLength = bI.length;
		mReferenceWeight = bI.refWeight;
		mReferenceLength = bI.refLength;
	}

	void pBone::DumpGlobalQuats(iostream& outFile)
	{
		outFile << mBoneName << endl;
		outFile << mGlobalRotation[0] << "\t";;
		outFile << mGlobalRotation[1] << "\t";;
		outFile << mGlobalRotation[2] << "\t";;
		outFile << mGlobalRotation[3] << "\t";;
		outFile << endl;
	}
	void pBone::DumpReferenceQuats(iostream& outFile)
	{
		outFile << mBoneName << endl;
		outFile << mReferenceRotation[0] << "\t";
		outFile << mReferenceRotation[1] << "\t";
		outFile << mReferenceRotation[2] << "\t";
		outFile << mReferenceRotation[3] << "\t";
		outFile << endl;
	}
	void pBone::LoadLocalQuats(iostream& inFile)
	{
		string assumedBoneName;
//		char tmpString[1024];
//		memset(tmpString,0,sizeof(tmpString));
//		inFile.getline(tmpString, 1023);
		getline(inFile, assumedBoneName);

		inFile >> mLocalRotation[0];
		inFile >> mLocalRotation[1];
		inFile >> mLocalRotation[2];
		inFile >> mLocalRotation[3];

		if (mBoneName != assumedBoneName)
		{
			// 
			// Error! The assumed name is not the same as 
			//
			throw string("Unexpected bone name");
			return;
		}

		//
		// Make sure we have a valid quaternion.
		//
		if (mLocalRotation[0] == 0 &&
			mLocalRotation[1] == 0 &&

			mLocalRotation[2] == 0 &&			mLocalRotation[3] == 0)
		{
			pVector4f_COPY(g_quatIdent, mLocalRotation);
		}
		pVectorNormalize4f	(mLocalRotation, mLocalRotation);
		//Update(true);

		inFile.ignore(2, '\n');
	}
	void pBone::LoadLocalQuats(const pQuat_d& quat)
	{
		// make sure quaternion is valid
		if(quat[0] == 0 && quat[1] == 0 && quat[2] == 0 && quat[3] == 0) {
			pVector4f_COPY(g_quatIdent, mLocalRotation);
		} else {
			pVector4f_COPY(quat, mLocalRotation);
		}
	}
	void pBone::LoadGlobalQuats(iostream& inFile)
	{
		string assumedBoneName;
//		char tmpString[1024];
//		memset(tmpString,0,sizeof(tmpString));
//		inFile.getline(tmpString, 1023);
		getline(inFile, assumedBoneName);

		inFile >> mGlobalRotation[0];
		inFile >> mGlobalRotation[1];
		inFile >> mGlobalRotation[2];
		inFile >> mGlobalRotation[3];

		if (mBoneName != assumedBoneName)
		{
			// 
			// Error! The assumed name is not the same as 
			//
			throw string("Unexpected bone name");
			return;
		}

		//
		// Make sure we have a valid quaternion.
		//
		if (mGlobalRotation[0] == 0 &&
			mGlobalRotation[1] == 0 &&
			mGlobalRotation[2] == 0 &&
			mGlobalRotation[3] == 0)
		{
			pVector4f_COPY(this->mReferenceRotation, mGlobalRotation);
		}
		pVectorNormalize4f	(mGlobalRotation,mGlobalRotation);

		// Add in the reference rotation to get the bone structure into it's default position.
//		pQuat InvReferenceRotation;
//		pQuatInverse(mReferenceRotation, InvReferenceRotation);
//		pQuatMultiply(mGlobalRotation, mReferenceRotation, mGlobalRotation);

		//
		// Calculate the local rotation given the parent global rotation,
		// and the provided global rotation for this joint.
		//
//		Update(false);
		pQuat				InvParentGlobalRotation;
		pQuatInverse		(mParentGlobalRotation, InvParentGlobalRotation);
		pQuatMultiply		(mGlobalRotation, InvParentGlobalRotation , mLocalRotation );
		//Update(true);

		inFile.ignore(2, '\n');
	}

	void pBone::Draw() const
	{
		glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_POINT_BIT);
		glDisable(GL_LIGHTING);
		pBone *lParent = this->GetParent();
		if (lParent != NULL)
		{
			glLineWidth(2);
			glPointSize(4);
			glBegin(GL_LINES);
				glColor3f(1.0f, 0.0f, 0.0f);
				if (mSelected)
					glColor3f(1.0f, 1.0f, 0.0f);
				// untransformed
				/*glVertex3fv(mProximal);
				glColor3f(0.0f, 1.0f, 0.0f);
				if (mSelected)
					glColor3f(0.0f, 1.0f, 1.0f);
				glVertex3fv(mDistal);*/

				glColor3f(1.0f, 0.0f, 1.0f);
				glVertex3fv(mTProximal);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3fv(mTDistal);
			glEnd();

			glColor3f(0.5f, 0.5f, 0.5f);
			glBegin(GL_POINTS);
				glVertex3fv(mProximal);
				glVertex3fv(mDistal);
			glEnd();
		}
		else
		{
			glPointSize(10);
			glColor3f(0.5f, 0.5f, 0.5f);
			glBegin(GL_POINTS);
				glVertex3fv(mDistal);
			glEnd();
			glPointSize(1);
		}

		glPopAttrib();
	}
	pBone::~pBone(void) 
	{
		int i;

	/*	if (!this->mInuse)
			return;*/
		
		for (i=0; i<MDP_MAX_MESHES; i++)
			if (mAttachedVertCount[i])
				gfree(mAttachedVerts[i]);

		if (mChildren)
			gfree(mChildren);
		
		return;
	}

	// Update the bone orientations
	void pBone::Update(bool iRecurse)
	{
		pVector3f ivNewDistal;		// Vector translation
		
		// If there is no parent, then use the identity quat to represent no global rotation
		if (mParent)
			pVector4f_COPY(mParent->mGlobalRotation, mParentGlobalRotation);
		else
			pVector4f_COPY(g_quatIdent, mParentGlobalRotation);

		// Move to the reference rotation
//		pQuatMultiply		(mReferenceRotation, mParentGlobalRotation	, mGlobalRotation	);
//		pVectorNormalize4f	(mGlobalRotation, mGlobalRotation							);
		// Multiply in the local rotation
//		pQuatMultiply		(mLocalRotation	, mGlobalRotation	, mGlobalRotation );
//		pVectorNormalize4f	(mGlobalRotation, mGlobalRotation							);

		pQuatMultiply		(mLocalRotation	, mParentGlobalRotation	, mGlobalRotation	);
		pVectorNormalize4f	(mGlobalRotation, mGlobalRotation							);

		// Calculate the original position of the distal joint
		pVector3f boneVector;
		pVector3f_ADD		(mProximal		, mBoneVector			, mDistal			);
		pVector3f_SCALE(mBoneVector, mLength/mReferenceLength, boneVector);			// For anthropometry

		if (mParent)
		{
			pQuatRotateVector	(mGlobalRotation, boneVector, ivNewDistal	);
			pVector3f_ADD		(ivNewDistal	, mTProximal , mTDistal		);
		}
		else 
			pVector3f_COPY( mDistal, mTDistal);	// Dont rotate if there is not proximal


		if (!iRecurse)
			return;

		// Update all children
		for (int i=0; i<(int)mChildrenCount; i++)
		{
			mChildren[i]->SetTProximal(mTDistal);		// Move children to end of current bone
			mChildren[i]->SetProximal(mDistal);		// Move children to end of current bone
			mChildren[i]->Update(iRecurse);			// Update their positions
		}

	}
	void pBone::AttachVertsToBone	(const int iMeshIndex, const int iMeshLength, const int iNumSelected, const GLuint* iSelection, const bool iSelect)
	{
	//	ASSERT(iMeshIndex < MDP_MAX_MESHES);

		int i;
		unsigned char *lSelectionArray = (unsigned char*) gmalloc(sizeof(unsigned char) * (iMeshLength + 1));
		memset(lSelectionArray,0,sizeof(unsigned char) * iMeshLength);
		// Make a new array in which we will store the list of all attached points... set everything to zero saying all
		// the points are NOT selected

		// Add the points that already ARE attached to this array [1 means they ARE valid vertexes]
		if (mAttachedVerts[iMeshIndex] != NULL)
		{
			for (i=0; i<(int)mAttachedVertCount[iMeshIndex]; i++)
			{
				#ifdef mdpDevelopmentBuild
					ASSERT(mAttachedVerts[iMeshIndex][i] <= iMeshLength);
				#endif
				lSelectionArray[mAttachedVerts[iMeshIndex][i]]	= 1;
			}
		}
		gfree(mAttachedVerts[iMeshIndex]);
		
		int lFinalCount=0;
		// Toggle selected verts
		for (i=0; i<iNumSelected; i++)
		{
			int whichIndex = (iSelection != NULL) ? iSelection[i] : i;
			#ifdef mdpDevelopmentBuild
				ASSERT(whichIndex <= iMeshLength);
			#endif					
			if (iSelect)
				lSelectionArray[whichIndex] = 1;
			else 
				lSelectionArray[whichIndex] = 0;
		}

		// Count selected verts, lFinalCount stores the NEW number to attach
		for (i=0; i<iMeshLength; i++)
		{
			if (lSelectionArray[i] != 0)
				lFinalCount ++;
		}

		// Create a new array, JUST big enough to store the list of attached vertexes
		mAttachedVertCount[iMeshIndex] = lFinalCount;

		if (mAttachedVertCount[iMeshIndex] == 0)
			mAttachedVerts[iMeshIndex] = NULL;
		else
		{
			mAttachedVerts[iMeshIndex] = (GLuint*) gmalloc(sizeof(GLuint) * lFinalCount);
			memset(mAttachedVerts[iMeshIndex],0,sizeof(GLuint) * lFinalCount);
		
			// Store the index numbers of the attached verts into this list
			for (i=iMeshLength-1; i>=0 && lFinalCount > 0; i--)
			{
				if (lSelectionArray[i] != 0)
					mAttachedVerts[iMeshIndex][--lFinalCount] = i;
			}
		}
			
		gfree(lSelectionArray);		// Free temporary memory
	}


	// Convert the pointers stored in (this) to indexes into the lump in which the data is stored
	// howver retain the 'type' information.
	void pBone::PointersToIndexes(int iTotal, pBone** iPointers)
	{
		if (mParent == NULL)
			mParent = (pBone*)-1;	// -1 means it doesnt exist
		for (int i=0; i<iTotal; i++)
		{
			if (iPointers[i] == mParent) { mParent = (pBone*)i; }
			for (int j=0; j<(int)mChildrenCount; j++)
			{
				if (iPointers[i] == mChildren[j]) mChildren[j] = (pBone*)i;
			}
		}
	}
	void pBone::IndexesToPointers(int iTotal, pBone** iPointers)
	{
		if ((int)mParent == -1)
			mParent = NULL;
		else
			mParent = iPointers[(int)mParent];

		if (!mChildren)
			return;
		for (int i=0; i<(int)mChildrenCount; i++)
		{
			if ((int)mChildren[i] < iTotal && (int)mChildren[i] >= 0)
				mChildren[i] = iPointers[(int)mChildren[i]];
		}
	}
	size_t pBone::size(void)
	{
		int lSerialLength = 0;
		int i;

		lSerialLength += mDataSize;
		for (i=0; i<MDP_MAX_MESHES; i++)
			lSerialLength += sizeof(GLuint) * mAttachedVertCount[i];
		lSerialLength += sizeof(pBone*) * mChildrenCount;

		return lSerialLength;
	}

	#define FILE_TO_LUMP(var, type, count) {											\
		const size_t lumpSize = count * sizeof(type);									\
		GLuint* lpVertArrayLocation = (GLuint*)((unsigned char*)iSerialLump + lOffset); \
		var = (type*) gmalloc(lumpSize);												\
		memcpy(var, lpVertArrayLocation, lumpSize);										\
		lOffset += lumpSize;															\
		}
	#define LUMP_TO_FILE(var, type, count) {							\
		memcpy(oSerialLump + lOffset, var, count * sizeof(type));		\
		lOffset += count * sizeof(type);								\
		}

	int	pBone::Serialize (unsigned char* &oSerialLump)
	{
		int i;
		int lSerialLength = this->size();
		int lOffset = 0;
		
		oSerialLump = (unsigned char*)gmalloc(lSerialLength * sizeof(unsigned char));

		// Save the member variables
		memcpy(oSerialLump, mDataStart, mDataSize);
		lOffset += mDataSize;

		// Serialize the list of verticies attached to this bone to the file
		for (i=0; i<MDP_MAX_MESHES; i++)
			if (mAttachedVertCount[i])
				LUMP_TO_FILE(mAttachedVerts[i], GLuint, mAttachedVertCount[i]);

		if (mChildren)
			LUMP_TO_FILE(mChildren,GLuint, mChildrenCount);

		return lOffset;
	}

	bool pBone::Deserialize (unsigned char* &iSerialLump)
	{
		int i;
		int lSerialLength = 0;
		int lOffset = 0;

		// TODO string objects should not be saved in binary to file but that is what this does
		// TODO after migrating to VS2012, the program threw an exception when destructing pBones
		// TODO the exception doesn't occur if we don't allow the first four bytes of the mBoneName string
		// TODO object to be clobbered by reading data in from the file
		// TODO However, this is only true in debug mode

//#ifdef _DEBUG
		// save first four bytes of mBoneName
//		int firstByteOfmBoneName = *((int*)&mBoneName);
//#endif

		memcpy(mDataStart,iSerialLump,mDataSize);

//#ifdef _DEBUG
		// restore first word of mBoneName
//		*((int*)&mBoneName) = firstByteOfmBoneName;
//#endif		
		mBoneName = mBoneNameCstr;

		lOffset +=mDataSize;
		// lOffset is now a pointer to the beginning of the attached verts list

		for (i=0; i<MDP_MAX_MESHES; i++)
			if (mAttachedVertCount[i])
				FILE_TO_LUMP(mAttachedVerts[i], GLuint, mAttachedVertCount[i]);

		if (mChildren)
			FILE_TO_LUMP(mChildren,pBone*, mChildrenCount);

		// Reset reference rotation, as it is determined on load.
		//pVector4f_COPY(g_quatIdent, this->mReferenceRotation);
		return true;
	}

}