#ifdef mdpDevelopmentBuild
	#include "dev_locals.h"
#else
	#include "locals.h"
#endif

#include <map>
using std::map; 
using std::iostream;
using std::string;

namespace perihelion	{

	// =======================================================================================
	// =======================================================================================
	// =======================================================================================
	// This is the base model file format for Perihelion.  It is a proprietary model file format
	// copyright 2002, Arthur C. Tomlin - All Rights Reserved.

	// Constructors
	MDP::MDP(void)								{	Create("", "");					}
	MDP::MDP(string fileName)					{	Create(fileName, "");			}
	MDP::MDP(string fileName, string skinName)	{	Create(fileName, skinName);		}
	// Actual construction function
	void MDP::Create(string iFileName, string iSkinName)
	{
		mDataStart					= &mDataStart + 1;
		mDataEnd					= &mDataEnd;
		mDataSize					= ((int)mDataEnd - (int)mDataStart);
		mNumMeshes = 0;
		mNumBones  = 0;
		mMeshes = NULL;
		mBones = NULL;
		mInuse = false;

		// If we're given a filename...
		if (!iFileName.empty())
			// Load the file
			if (!LoadFromFile(iFileName))
			{
				// Uh-Oh
	#ifdef mdpDevelopmentBuild
				Sys_Error("Could not load MDP: \"%s\"",iFileName.c_str());
	#else
				throw string("Could not load MDP");
	#endif
			}
			else
				this->mInuse = true;

		if (!iSkinName.empty())
		{
			;	// TODO, load skin
		}

	
		return;
	}
	void MDP::AttachMeshToBone(int boneIndex, int meshIndex)
	{
		if (meshIndex < 0 || meshIndex > mNumMeshes)
			return;

		mBones[boneIndex]->AttachVertsToBone(meshIndex, mMeshes[meshIndex-1]->GetLength(), mMeshes[meshIndex-1]->GetLength(), NULL, 1);
	}
	void MDP::Draw(bool drawBones, bool drawMeshs) const
	{
		int i;


		// Draw the bone system
		// if (editor_drawBones.value != 0)
		if (drawBones)
			for (i=0;i<mNumBones;i++)
			{
				mBones[i]->Draw();
			}

		// Draw the meshes
		// if (editor_drawMeshes.value != 0)
		if (drawMeshs)
			for (i=0;i<mNumMeshes;i++)
			{
				mMeshes[i]->Draw();
			}
			
	}
	// Constant
	bool MDP::Save(string fileName, int writeFlags) const
	{
		// Dump the entire model to a file here
		return true;
	}

	// Mutators
	bool MDP::LoadSkin(string skinName) 
	{
		// TODO - Load MDP Skin
		return true;
	}
	bool MDP::UnloadSkin()
	{
		// TODO - Unload MDP Skin
		return true;
	}
	// Mutators
	bool MDP::SetJointAngles(pQuat	  jointList[])
	{
	return true;
	}
	bool MDP::SetLinkLengths(float lengthList[])
	{
	return true;
	}
	bool MDP::SetWeight(float weight)
	{
	return true;
	}
	GLuint MDP::AddMesh(pMesh* newMesh, pVector3f translation, pQuat rotation)
	{
		// Ensure there is only one copy of this mesh in this object
		for(int i=0;i<mNumMeshes;i++)
		{
			if (mMeshes[i] == newMesh)
				return 0;
		}

		newMesh->SetTranslation(translation);
		newMesh->SetRotation(rotation);

		// Realocate the mesh list
		pMesh** newMeshList = (pMesh**) gmalloc(sizeof(pMesh*) * (mNumMeshes+1));
		if (mNumMeshes > 0)
		{
			memcpy(newMeshList,mMeshes,sizeof(pMesh*) * (mNumMeshes));
			gfree(mMeshes);
		}
		newMeshList[mNumMeshes] = newMesh;	// tack the new mesh onto the end of the new list
		
		mMeshes = newMeshList;				// make the new list THE list
		mNumMeshes++;						// increase the count to reflect the new mesh
		return mNumMeshes;
	}
	void MDP::DumpBoneAttributes(iostream &outFile)
	{
		for(int i=0; i<mNumBones;i++)
		{
			this->mBones[i]->DumpInfo(outFile);
			this->mBones[i]->DumpReferenceQuats(outFile);
		}
	}
	void MDP::GetJointDistalByName(string boneName, pVector3f &out)
	{
		pVector3f_COPY(g_origin3f, out);
		for(int i=0; i<mNumBones;i++)
		{
			if (mBones[i]->GetName() == boneName)
			{
				pVector3f_COPY(mBones[i]->GetTDistal(), out);
				return;
			}
		}
	}
	void MDP::GetJointProximalByName(string boneName, pVector3f &out)
	{
		pVector3f_COPY(g_origin3f, out);
		int what = mNumBones;
		for(int i=0; i<mNumBones;i++)
		{
			if (mBones[i]->GetName() == boneName)
			{
				pVector3f_COPY(mBones[i]->GetTProximal(), out);
				return;
			}
		}
	}
	void MDP::LoadBoneAttributes(iostream &inFile, bool updateName)
	{		
		
		for(int i=0; i<mNumBones;i++)
		{
			this->mBones[i]->LoadInfo(inFile, updateName);
			this->mBones[i]->LoadLocalQuats(inFile);
		}
	}
	void MDP::LoadBoneAttributes(std::map<string, boneInfo> boneInfoMap, bool updateName) {
		for(int i=0; i < mNumBones; i++) {
			this->mBones[i]->LoadInfo(boneInfoMap[mBones[i]->GetName()], updateName);
			this->mBones[i]->LoadLocalQuats(boneInfoMap[mBones[i]->GetName()].q);
		}
	}

	void MDP::Destroy(void) 
	{
		int i;

		if (!mInuse)
			return;
		
		
		for (i=0;i<mNumMeshes;i++)
			if (mMeshes[i])
				delete mMeshes[i];
		for (i=0;i<mNumBones;i++)
			if (mBones[i])
				delete mBones[i];

		mNumMeshes = 0;
		mNumBones  = 0;
		mSelectedBoneIndex = -1;

		gfree(mBones);
		gfree(mMeshes);
		mInuse = false;
	}
	// Destructor
	MDP::~MDP(void) 
	{
		Destroy();
	}

	GLuint MDP::UnselectBone()
	{
		// Unselect if selected
		if (mSelectedBoneIndex >= 0)
			mBones[mSelectedBoneIndex]->mSelected = false;
		mSelectedBoneIndex = -1;
		return mSelectedBoneIndex;
	}

	// Select a bone by its index number
	GLuint MDP::SelectBone(int		iIndex)
	{
		iIndex --;		// User numbers bones 1..MAX, so we subtract one to get equiv array index
		if (iIndex<mNumBones && iIndex>=0)
		{
			UnselectBone();
			mSelectedBoneIndex = iIndex;
			mBones[iIndex]->mSelected = true;
			UpdateSelectedBoneVertColors();
		}
		return mSelectedBoneIndex;
	}

	// Select a bone by proximity to a 3D location
	GLuint MDP::SelectBone(pVertex	iPoint)
	{
		double minDistance = -1;
		int minIndex = -1;
		for (int i=0;i<mNumBones;i++)
		{
			double VecLength;
			pVertex Difference;
			pVector3f_SUBTRACT(iPoint, mBones[i]->GetDistal(), Difference);
			VecLength = pVectorLength(Difference);

			if (VecLength < minDistance || minDistance < 0)
			{
				minIndex = i;
				minDistance = VecLength;
			}
		}
		if (minIndex > -1)
		{
			UnselectBone();
			mSelectedBoneIndex = minIndex;
			mBones[minIndex]->mSelected = true;
			UpdateSelectedBoneVertColors();
			return mSelectedBoneIndex;
		}
		return 0;
	}
	void MDP::SelectChildBone(int which)
	{
		if (mSelectedBoneIndex != -1)
		{
			pBone* tmpBone = mBones[mSelectedBoneIndex]->GetChild(which);
			if (tmpBone != NULL)
			{
				UnselectBone();
				SelectBone(tmpBone);
			}
		}
	}
	GLuint MDP::SelectBone(pBone*	iPtr)
	{
		for (int i=0;i<mNumBones;i++)
			if (mBones[i] == iPtr)
			{
				UnselectBone();
				mSelectedBoneIndex = i;
				mBones[i]->mSelected = true;

				UpdateSelectedBoneVertColors();
			}
		return mSelectedBoneIndex;
	}
	void MDP::UpdateSelectedBoneVertColors() 
	{
		if (mSelectedBoneIndex == 0)
			return;

		// For every mesh, find the verts that are attached to the currently selected bone, and tell the
		// mesh to hilight these verts.
		for (int meshNum = 1; meshNum <= GetMeshCount(); meshNum++)
		{
			GLuint *indexes;
			int vertCount = this->mBones[mSelectedBoneIndex]->GetAttachedVertList(meshNum, &indexes);
			GetMesh(meshNum)->SetSelectedBoneVertlist(vertCount, (int*)indexes);
			
		}
	}

	void MDP::UpdateVertexColors(float r, float g, float b)
	{
		for (int i=0; i<mNumMeshes; i++)
		{
			mMeshes[i]->UpdateVertexColors(r,g,b);
		}		
	}
	void MDP::UpdateVertexShirtColors(float r, float g, float b, bool male, std::vector<int> & maleShirt, std::vector<int> & femaleShirt)
	{
		mMeshes[0]->UpdateVertexShirtColors(r, g, b, male, maleShirt, femaleShirt);
	}
	void MDP::UpdateVertexPantColors(float r, float g, float b, bool male, std::vector<int> & malePant, std::vector<int> & femalePant)
	{
		mMeshes[0]->UpdateVertexPantColors(r, g, b, male, malePant, femalePant);
	}
	void MDP::UpdateBones(void)
	{
		int i;
		// Update the root bones, recursivly
		for (i=0;i<mNumBones;i++)
		{
			if (mBones[i]->GetParent() == NULL)
				mBones[i]->Update(true);
		}
	}

	void MDP::UpdateVerts(void)
	{
		int i,j;
			float *lqChild, *lqParent, *lqRef;

		for (j=0; j<mNumMeshes; j++)
		{
			mMeshes[j]->Update();
			//
			for (i=0;i<mNumBones;i++)
			{
				int lCount;
				GLuint *lIndexes;
				lCount = mBones[i]->GetAttachedVertList(j+1, &lIndexes);
				lqParent = mBones[i]->GetParentRotation();
				lqRef    = mBones[i]->GetRefRotation();
				lqChild	 = mBones[i]->GetLocalRotation();
				mMeshes[j]->RotateVertListByQuats(mBones[i]->GetLengthScale(),mBones[i]->GetWeightScale(), mBones[i]->GetProximal(), mBones[i]->GetDistal(), mBones[i]->GetTProximal(), lqParent, lqChild, lqRef, lCount, lIndexes);
			}
		}	
	}
	typedef struct MDPHeader_s
	{
		short fileType1;
		short fileType2;
		int version;
	} MDPHeader_t;

	/*
	#ifdef _DEBUG
	int SENTINAL_STRUCT[] = { 0xABCDEF12, 0x42837421, 0xF1283134 };
	int TEMP_STRUCT[]     = { 0, 0, 0 };
	#define ADD_SENTINAL AddSentinal(lOutFile);
	#define CHECK_SENTINAL CheckSentinal(lInFile);
	void AddSentinal(FILE* lOutFile) { fwrite(SENTINAL_STRUCT,sizeof(int)*3,1,lOutFile); }
	void CheckSentinal(FILE* lInFile) { 
		fread(TEMP_STRUCT,sizeof(int)*3,1,lInFile); 
		if (memcmp(TEMP_STRUCT,SENTINAL_STRUCT,sizeof(int)*3)) {
			Sys_FatalError("BAD SENTINAL IN FILE\n"); 
		}
		return;
	}
	#else
	*/
	#define ADD_SENTINAL
	#define CHECK_SENTINAL 
	//#endif

	bool MDP::SaveToFile(string iFileName) 
	{
		FILE *lOutFile;
		int i;

		
	// ###### Open The File ######

		lOutFile = fopen(iFileName.c_str(),"wb");
		if (!lOutFile)
		{
	//			Sys_Error("Could not open \"%s\" to output MDP to.", iFileName.c_str());
			return false;
		}

	// ###### Write the header ######

		MDPHeader_t myMDPHeader = {
			('M' << 8 | 'D'), 
			('P' << 8 | '1'),
			MDP_VERSION,
		};
		fwrite(&myMDPHeader, sizeof(myMDPHeader), 1, lOutFile);
		fwrite(mDataStart, mDataSize, 1, lOutFile);

	// ###### Get Serialized Memory Lumps ######

		unsigned char **lMeshLump = (unsigned char**) gmalloc(mNumMeshes * sizeof (unsigned char *));	// create mNumMeshes pointers
		unsigned char **lBoneLump = (unsigned char**) gmalloc(mNumBones  * sizeof (unsigned char *));	// create mNumBones  pointers
		int	*lMeshLumpSize = (int*) gmalloc(mNumMeshes * sizeof (int));
		int	*lBoneLumpSize = (int*) gmalloc(mNumBones * sizeof (int));
		memset (lMeshLump, 0, mNumMeshes * sizeof (unsigned char *));
		memset (lBoneLump, 0, mNumBones  * sizeof (unsigned char *));
		memset (lMeshLumpSize, 0, mNumMeshes * sizeof (int));
		memset (lBoneLumpSize, 0, mNumBones  * sizeof (int));

		ADD_SENTINAL

		// Get the serialized lumps for each mesh
		for (i = 0; i<mNumMeshes; i++)
		{
			lMeshLumpSize[i] = mMeshes[i]->Serialize(lMeshLump[i]);
		}

		// Get the serialized lumps for each bone
		for (i = 0; i<mNumBones; i++)
		{
			mBones[i]->PointersToIndexes(mNumBones, mBones);
			lBoneLumpSize[i] = mBones[i]->Serialize(lBoneLump[i]);
			mBones[i]->IndexesToPointers(mNumBones, mBones);			// restore pointers
		}

	// ##### Write Lump Headers #####

		int lCurrentOffset = 0;

		// Offsets written are for the END of the lump
		for (i = 0; i<mNumMeshes; i++)
		{
			lCurrentOffset += lMeshLumpSize[i];
			fwrite(&lCurrentOffset, sizeof(int), 1, lOutFile);
			
		}
		ADD_SENTINAL
		for (i = 0; i<mNumBones; i++)
		{
			lCurrentOffset += lBoneLumpSize[i];
			fwrite(&lCurrentOffset, sizeof(int), 1, lOutFile);
		}

		ADD_SENTINAL
	// ###### Write Lumps ######
	/*
		fpos_t pos;
		
		fgetpos(lOutFile,&pos);
		lCurrentOffset = pos;

	*/
		// This is the point where we START all 'pointer' offsets from [aka, where the lCurrentOffset points to]
		for (i = 0; i < mNumMeshes; i++)
		{
			fwrite(lMeshLump[i], sizeof(unsigned char), lMeshLumpSize[i], lOutFile);
		}
		ADD_SENTINAL
		for (i = 0; i < mNumBones; i++)
		{
			fwrite(lBoneLump[i], sizeof(unsigned char), lBoneLumpSize[i], lOutFile);
		}
		ADD_SENTINAL
		fflush(lOutFile);
		fclose(lOutFile);

	// ###### Local Mem Cleanup ######
		// Free the memory allocated for the serial lumps
		for (i = 0; i<mNumMeshes; i++)
			gfree(lMeshLump[i]);
		for (i = 0; i<mNumBones; i++)
			gfree(lBoneLump[i]);

		// Free the memory allocated for the lump index arrays
		gfree(lMeshLump);
		gfree(lBoneLump);
		gfree(lMeshLumpSize);
		gfree(lBoneLumpSize);

		return true;
	}
	 
	bool MDP::LoadFromFile(string iFileName, bool updateReference) 
	{
		
		FILE *lInFile;
		int i;
		
		// Clean up existing MDP
		if (this->mInuse)
			this->Destroy();

	// ###### Open The File ######

		//lInFile = fopen(iFileName.c_str(),"rb");
		fopen_s(&lInFile, iFileName.c_str(), "rb");
		if (!lInFile)
		{
	//		Sys_Error("Could not open \"%s\" to read MDP from.", iFileName.c_str());
			return false;
		}

	// ###### Read the entire thing in one big chunk ######
		// Get the length of the file
		fpos_t lFileLength;
		fseek(lInFile, 0, SEEK_END);
		lFileLength = ftell(lInFile);
		fseek(lInFile, 0, SEEK_SET);
		if (!(lFileLength > 0 /* && lFileLength < MAX FREE RAM */))
		{
			fclose(lInFile);
			return false;
		}
		

		// TODO - check for proper version...
		MDPHeader_t myMDPHeader;

		fread(&myMDPHeader, sizeof(myMDPHeader), 1, lInFile);
		fread(mDataStart, mDataSize, 1, lInFile);

	//	unsigned char *lOffset = sizeof(myMDPHeader);
		mInuse = true;
		
		
		unsigned char **lMeshLump = (unsigned char**) gmalloc(mNumMeshes * sizeof (unsigned char *));	// create mNumMeshes pointers
		unsigned char **lBoneLump = (unsigned char**) gmalloc(mNumBones  * sizeof (unsigned char *));	// create mNumBones  pointers

		// Create the memory needed for all the pMeshes and pBones
		mMeshes = (pMesh**)gmalloc(mNumMeshes *sizeof (pMesh*));
		mBones	= (pBone**)gmalloc(mNumBones  *sizeof (pBone*));
		memset (lMeshLump, 0, mNumMeshes  * sizeof (unsigned char *));
		memset (lBoneLump, 0, mNumBones  * sizeof (unsigned char *));

		// Create arrays which list the offsets of the lumps
		int	*lMeshLumpOffset = (int*) gmalloc(mNumMeshes  * sizeof (int*));
		int	*lBoneLumpOffset = (int*) gmalloc(mNumBones  * sizeof (int*));
		memset (lMeshLumpOffset, 0, mNumMeshes  * sizeof (int));
		memset (lBoneLumpOffset, 0, mNumBones  * sizeof (int));

		CHECK_SENTINAL
		// Load the lists of lump offsets
		fread(lMeshLumpOffset, sizeof(int), mNumMeshes, lInFile);	
		CHECK_SENTINAL
		fread(lBoneLumpOffset, sizeof(int), mNumBones, lInFile);
		CHECK_SENTINAL

		int lPrevOffset = 0;
		int lOffset = lPrevOffset;
		int lLength = 0;
		for (i = 0; i<mNumMeshes; i++)
		{
			lPrevOffset = lOffset;
			lOffset = lMeshLumpOffset[i];
			lLength = lOffset - lPrevOffset;

			// make room for this lump in ram
			lMeshLump[i] = (unsigned char*) gmalloc(lLength * sizeof(unsigned char));
			// And read the lump into memory

	#ifdef mdpDevelopmentBuild
				mMeshes[i]	= new pDevMesh;
	#else
				mMeshes[i] = new pMesh;
	#endif

			fread(lMeshLump[i],sizeof(unsigned char), lLength, lInFile);
			this->mMeshes[i]->Deserialize(lMeshLump[i]);
		}
		CHECK_SENTINAL
		for (i = 0; i<mNumBones; i++)
		{
			lPrevOffset = lOffset;
			lOffset = lBoneLumpOffset[i];
			lLength = lOffset - lPrevOffset;

			// make room for this lump in ram
			lBoneLump[i] = (unsigned char*) gmalloc(lLength * sizeof(unsigned char));
			// And read the lump into memory
	#ifdef mdpDevelopmentBuild
			mBones[i]	= new pDevBone;
	#else
			mBones[i]	= new pBone;
	#endif
			fread(lBoneLump[i],sizeof(unsigned char), lLength, lInFile);
			this->mBones[i]->Deserialize(lBoneLump[i]);
			if (updateReference)
				mBones[i]->UseCurrentRotationAsReference();
		}

		// Once all the bones have been created, fix the 'pointers'
		for (i = 0; i<mNumBones; i++)
			this->mBones[i]->IndexesToPointers(mNumBones, mBones);

		CHECK_SENTINAL


		// read in the color assignment 
		int numBoneVertShirt;
		int numBoneVertPant;

		// read the number of vertices that should be assigned shirt color
		fread(&numBoneVertShirt, sizeof(int), 1, lInFile);
		// create room in ram 
		int * shirtVertList = (int *) gmalloc(numBoneVertShirt * sizeof(int));
		// read the vertices into ram
		fread(shirtVertList, sizeof(int), numBoneVertShirt, lInFile);
		// transfer the vertices into a vector
		for(int i = 0; i < numBoneVertShirt; ++i) {
			bonelistShirt.push_back((int)shirtVertList[i]);
		}
		
		// read the number of vertices that should be assigned pant color
		fread(&numBoneVertPant, sizeof(int), 1, lInFile);
		// create room in ram 
		int * pantVertList = (int *) gmalloc(numBoneVertPant * sizeof(int));
		// read the vertices into ram
		fread(pantVertList, sizeof(int), numBoneVertPant, lInFile);
		// transfer the vertices into a vector
		for(int i = 0; i < numBoneVertPant; ++i) {
			bonelistPant.push_back((int)pantVertList[i]);
		}


		
		// ###### Mem Cleanup ######

		// Free the memory allocated for the serial lumps
		for (i = 0; i<mNumMeshes; i++)
			gfree(lMeshLump[i]);
		for (i = 0; i<mNumBones; i++)
			gfree(lBoneLump[i]);

		// Free the memory allocated for the lump index arrays
		gfree(lMeshLump);
		gfree(lBoneLump);
		gfree(lMeshLumpOffset);
		gfree(lBoneLumpOffset);

		// Free the memory allocated for color assignment
		gfree(shirtVertList);
		gfree(pantVertList);

	//*/
	//	this->Destroy();
		fclose(lInFile);
		return true;
	}


}	// End namespace perihelion
