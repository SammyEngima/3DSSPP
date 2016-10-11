#ifndef MODEL_MDP_H
#define MODEL_MDP_H

#pragma once
#include <map>
#include "math_vector.h"
#include "math_quaternion.h"
#include "g_types.h"
#include <vector>

namespace perihelion {
	class pBone;
	class pMesh;
}
class boneInfo;

namespace perihelion
{

	// =======================================================================================
	// This is the base model file format for Perihelion.  It is a proprietary model file format
	// copyright 2002, Arthur C. Tomlin - All Rights Reserved.
	class MDP 
	{
		friend pMesh;
	public:
		// Constructors
		MDP(void);								
		MDP(std::string fileName);					
		MDP(std::string fileName, std::string skinName);	
		void Create(std::string fileName, std::string skinName);	// Actual construction function

		// Constant
		bool Save(std::string fileName, int writeFlags) const;
		void Draw(bool drawBones=true, bool drawMeshs=true) const;								// Draws entire model

		// Accessors
		void DumpBoneAttributes(std::iostream &outFile);
		void LoadBoneAttributes(std::iostream &inFile, bool updateName=true);
		void LoadBoneAttributes(std::map<std::string, boneInfo> boneInfoMap, bool updateName=true);
		bool LoadSkin(std::string skinName);
		bool UnloadSkin();
		bool Inuse() { return mInuse; }
	#ifdef mdpDevelopmentBuild
		inline pDevMesh* GetMesh(int index) const { return (pDevMesh*) mMeshes[index-1]; }
		inline pDevBone* GetBone(int index) const { return (pDevBone*) mBones[index-1]; }
		inline pDevBone* GetSelectedBone()	const { 
			if (mSelectedBoneIndex >= 0) { 
				return (pDevBone*) mBones[mSelectedBoneIndex];
			} else { 
				return NULL; 
			}
		}
	#else
		void GetJointProximalByName(std::string boneName, pVector3f &out);
		void GetJointDistalByName(std::string boneName, pVector3f &out);
		inline pMesh* GetMesh(int index) const { return (pMesh*) ((index > 0 && index <= mNumMeshes) ? mMeshes[index-1] : NULL); }
		inline pBone* GetBone(int index) const { return (pBone*) ((index > 0 && index <= mNumMeshes) ? mBones[index-1] : NULL); }
		inline pBone* GetSelectedBone()	const { 
			if (mSelectedBoneIndex >= 0) { 
				return (pBone*) mBones[mSelectedBoneIndex];
			} else { 
				return NULL; 
			}
		}
	#endif


		// Mutators
		bool SetJointAngles(pQuat	  jointList[]);
		bool SetLinkLengths(float lengthList[]);
		bool SetWeight(float weight);
		void UpdateSelectedBoneVertColors();
		void UpdateVertexColors(float r, float g, float b);
		void UpdateVertexShirtColors(float r, float g, float b, bool male, std::vector<int> & maleShirt, std::vector<int> & femaleShirt);
		void UpdateVertexPantColors(float r, float g, float b, bool male, std::vector<int> & malePant, std::vector<int> & femalePant);

		GLuint SelectBone(int		iIndex);
		GLuint SelectBone(pVertex	iPoint);
		GLuint SelectBone(pBone*	iPtr);
		void SelectChildBone(int which);
		GLuint UnselectBone(void);

		GLuint AddMesh(pMesh* newMesh, pVector3f translation = g_origin3f, pQuat rotation = g_quatIdent);				// Add a mesh to the model
		void AttachMeshToBone(int boneIndex, int meshIndex);

		// Destructor
		~MDP(void);
		void Destroy(void);

		void	UpdateBones(void);
		void	UpdateVerts(void);
		inline const int GetMeshCount(void) const { return mNumMeshes; }
		inline const int GetBoneCount(void) const { return mNumBones; }
		bool SaveToFile(std::string iFileName);
		bool LoadFromFile(std::string iFileName, bool updateReference=false);

		std::vector<int> bonelistShirt;
		std::vector<int> bonelistPant;

	protected:
		size_t		mDataSize;

		// START TO COUNT IN SIZE
		void*		mDataStart;

		bool mInuse;
		int	mNumMeshes;
		int	mNumBones;
		int	mSelectedBoneIndex;

		
		pBone   **mBones;				// Bone definitions [connected to at least one joint] [attached meshes can be in renderLists]
		pMesh	**mMeshes;				// Mesh definitions [list of containing verticies]

		// END TO COUNT IN SIZE
		void*		mDataEnd;			// nothing after this point will be loaded/safed

	};

}

#endif
