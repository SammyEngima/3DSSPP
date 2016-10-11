#ifndef MODEL_BONE_H
#define MODEL_BONE_H

#pragma once

class boneInfo;
#define MAX_NAME_LENGTH 32

namespace perihelion
{

	// =======================================================================================
	class pBone
	{
	public:
		// Constructors
					pBone(void);
					pBone(pVertex iDistal);
					pBone(pVertex iDistal, pVector3f iRefVect);
			void	Initialize(void);
		// Destructor
					~pBone(void);

		// Constant
			void	Draw(void) const;								// Draws entire model
			size_t	size(void);										// Serialized lump size
		// Mutators
			void	Update				(bool		iRecurse);	// Update rotation of bones/meshes underneath this bone
	inline	void	SetOrientation		( pQuat		iQuat	)		{ pVector4f_COPY(iQuat,	mLocalRotation	);						} 
	inline	void	SetVector			( pVector3f	iVec	)		{ pVector3f_COPY(iVec ,	mBoneVector		);		/*/Update(true);/*/	}
	inline	void	SetProximal			( pVertex	iProx	)		{ pVector3f_COPY(iProx,	mProximal		);		/*/Update(true);/*/	}
	inline	void	SetTProximal		( pVertex	iProx	)		{ pVector3f_COPY(iProx,	mTProximal		);		/*/Update(true);/*/	}
	inline	void	SetDistal			( pVertex	iDist	)		{ pVector3f_SUBTRACT(iDist,mProximal,mBoneVector);	/*/Update(true);/*/	}
	inline	void	UseCurrentRotationAsReference()					{ pVector4f_COPY(mLocalRotation, mReferenceRotation);			}

		// Accessors
	inline  pBone*	GetChild			(GLuint		index	) const { return (index < mChildrenCount) ? mChildren[index] : NULL;			}
	inline  int     GetChildCount		()					  const { return mChildrenCount;				}
	inline  pBone*	GetParent			(void				) const { return mParent;						}
	inline	float*	GetVector			(void				) const	{ return (float*)mBoneVector;			}
	inline	float*	GetProximal			(void				) const	{ return (float*)mProximal;				}
	inline	float*	GetTProximal		(void				) const	{ return (float*)mTProximal;			}
	inline	float*	GetDistal			(void				) const	{ return (float*)mDistal;				}
	inline	float*	GetRefRotation		(void				) const	{ return (float*)mReferenceRotation;	} 
	inline	float*	GetLocalRotation	(void				) const	{ return (float*)mLocalRotation;		} 
	inline	float*	GetParentRotation	(void				) const	{ return (float*)mParentGlobalRotation;	} 
	inline	float*	GetGlobalRotation	(void				) const	{ return (float*)mGlobalRotation;	} 
	inline  float*  GetTDistal          (void               ) const { return (float*)mTDistal;          } 
	std::string  GetName             (void               ) const { return mBoneName; }
			void	AttachVertsToBone	(const int iMeshIndex, const int iMeshLength, const int iNumSelected, const GLuint* iSelection, const bool iSelect);
	inline	int		GetAttachedVertList	(const int iMeshIndex, GLuint** oIndexes) { *oIndexes = mAttachedVerts[iMeshIndex]; return mAttachedVertCount[iMeshIndex]; }
			int		Serialize			(unsigned char* &oSerialLump);
			bool	Deserialize			(unsigned char* &oSerialLump);
			void	PointersToIndexes			(int iTotal, pBone** iPointers);
			void	IndexesToPointers			(int iTotal, pBone** iPointers);
			void	LoadInfo(std::iostream &inFile, bool updateName=true);
			void	LoadInfo(const boneInfo& bI, bool updateName=true);
			void	DumpInfo(std::iostream &outFile);
			void	LoadGlobalQuats(std::iostream& inFile);
			void	DumpGlobalQuats(std::iostream& outFile);
			void	LoadLocalQuats (std::iostream& inFile);
			void	LoadLocalQuats(const pQuat_d& quat);
			void	DumpReferenceQuats(std::iostream& outFile);

			float   GetLengthScale() { return mLength / mReferenceLength; }
			float   GetWeightScale() { return mWeight / mReferenceWeight; }
	protected:
		std::string mBoneName;
		size_t		mDataSize;
		void*		mDataStart;				
		// NOTHING BEFORE THIS POINT WILL BE SAVED
		pVertex		mProximal, mDistal;		// Proximal is SET by PARENT, Distal is calculated
		pVector3f	mBoneVector;
		pVertex		mTProximal, mTDistal;
		pQuat		mReferenceRotation;		// Local rotation directly from saved file - stored internally as a reference posture
		pQuat		mGlobalRotation;		// Calculated from localRotation and parent globalRotation
		pQuat		mParentGlobalRotation;
		pBone		**mChildren;			// Array of pointers to children bones
		GLuint		mChildrenCount;
		bool		mInuse;
		pQuat		mLocalRotation;			
		pQuat		mLocalMin;				// Limit of rotation for flexion
		pQuat		mLocalMax;				// Limit of rotation for extension
		pBone		*mParent;				// Parent bone pointer [for forced bone removal mParent->DelChild(this);]
		GLuint	*mAttachedVerts[MDP_MAX_MESHES+1];
		GLuint	mAttachedVertCount[MDP_MAX_MESHES+1];

		// TODO in release build, strings only have 24 bytes, so we need to read in the extra four bytes that are in the file with an extra variable
//#ifndef _DEBUG
//		int extraBoneNameData;
//#endif
		//std::string		mBoneName;
		char		mBoneNameCstr[MAX_NAME_LENGTH];
		float		mWeight;
		float		mLength;
		float		mReferenceWeight;
		float		mReferenceLength;
	public:
		bool		mSelected;
	};

}

#endif
