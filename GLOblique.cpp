#include "GLOblique.h"
#include "GLSkeleton.h"
#include "glHom/locals.h"
#include "AnthroPercentiles.h"
#include "Main_Frm.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "CQuaternion.h"
#include "Skeleton.hpp"
#include "resource.h"
#include "enums.h"
#include "math.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include <strsafe.h>
#include <olectl.h> 
#include <ole2.h>
#include <sstream>
#include "enums.h"
#include <string>
#include <algorithm>
#include "Analysis.h" // for RightVerticalAngle() etc
#include "AVIExporter.h"
#include "glHom/model_clothing.h"
//#include "StatusBox.h" // TODO, for debugging, take out
using namespace perihelion;
using std::map;
using std::list;
using std::stringstream;
using std::string;
using std::find;

#define DTOR (3.14159265358979323 / 180.0)

bool GLOblique::showFrameText = false;

GLOblique::GLOblique(CWnd* owner) : GLInteractive(owner) {

	LoadHominoidMDP();
	currentMDP = &maleMDP;

	modelType = MT_FLESH;
	//shoesEnabled = true;
	floorEnabled = false;
	camera = userCamera = Camera(0.0,0.0,2290,50,20,-20);
	handheld = HandHeldObject(HHO_NONE,true,305,305);
	bgPicture = NULL;
	bgTexture = 0;
	bgMode = PM_STRETCH;
	bgActive = false;
	bgPostUpdate = false;
	bgAlpha = 0.0f;
	mouseDown = false;
}

void GLOblique::LoadHominoidMDP() {
	TCHAR filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);
	#ifdef _DEBUG
	PathRemoveFileSpec(filename);	// Back to debug directory
	#endif
	PathRemoveFileSpec(filename);	// Back to main directory
	if(this->pDoc->getMaleHominoidFilePresent()) {
		StringCchCatN(filename, MAX_PATH, "\\MaleHominoid.mdp", strlen("\\MaleHominoid.mdp"));
		maleMDP.LoadFromFile(filename, true);
		PathRemoveFileSpec(filename);	// Back to main directory

		// log color assignment
		MaleBoneListShirt.assign(maleMDP.bonelistShirt.begin(), maleMDP.bonelistShirt.end());
		MaleBoneListPant.assign(maleMDP.bonelistPant.begin(), maleMDP.bonelistPant.end());

	}
	if(this->pDoc->getFemaleHominoidFilePresent()) {
		StringCchCatN(filename, MAX_PATH, "\\FemaleHominoid.mdp", strlen("\\FemaleHominoid.mdp"));
		femaleMDP.LoadFromFile(filename, true);
		PathRemoveFileSpec(filename);	// Back to main directory

		// log color assignment
		FemaleBoneListShirt.assign(femaleMDP.bonelistShirt.begin(), femaleMDP.bonelistShirt.end());
		FemaleBoneListPant.assign(femaleMDP.bonelistPant.begin(), femaleMDP.bonelistPant.end());
	}

	if(!maleMDP.Inuse() || !femaleMDP.Inuse()) {
		// The document was wrong, the files are not available.
		std::string HominoidFailureString = 
			"The program was not able to load all hominoid models.  "
			"These files must be in the same directory as the 3DSSPP "
			"executable.  The oblique view cannot display the male or "
			"female model if the corresponding file is not loaded.  "
			"Please reinstall 3DSSPP or contact the University "
			"of Michigan to acquire these files:\n";

		if(!maleMDP.Inuse()) {
			HominoidFailureString += "\nMaleHominoid.mdp";
			pDoc->setMaleHomoidFilePresent(false);
		}
		if(!femaleMDP.Inuse()) {
			HominoidFailureString += "\nFemaleHominoid.mdp";
			pDoc->setFemaleHominoidFilePresent(false);
		}

		AfxMessageBox(	HominoidFailureString.c_str(), MB_OK );
	}

	if(pDoc->getShoeFilePresent()) {
		//
		// Load shoes
		//
		StringCchCatN(filename, MAX_PATH, "\\shoe.obj", strlen("\\shoe.obj"));
		pMesh* shoeMeshMaleLeft = NULL;
		pMesh* shoeMeshMaleRight= NULL;
		pMesh* shoeMeshFemaleLeft= NULL;
		pMesh* shoeMeshFemaleRight = NULL;
		if(maleMDP.Inuse()) {
			shoeMeshMaleLeft = new perihelion::pMesh;
			shoeMeshMaleRight = new perihelion::pMesh;
			shoeMeshMaleLeft->LoadOBJ(filename);
			shoeMeshMaleRight->LoadOBJ(filename);
		}
		if(femaleMDP.Inuse()) {
			shoeMeshFemaleLeft = new perihelion::pMesh;
			shoeMeshFemaleRight = new perihelion::pMesh;
			shoeMeshFemaleLeft->LoadOBJ(filename);
			shoeMeshFemaleRight->LoadOBJ(filename);
		}
	
		if (!maleMDP.Inuse() || !femaleMDP.Inuse() ||
			(maleMDP.Inuse() && (!shoeMeshMaleLeft->Inuse() ||
			!shoeMeshMaleRight->Inuse())) 
			||
			(femaleMDP.Inuse() && (!shoeMeshFemaleLeft->Inuse() ||
			!shoeMeshFemaleRight->Inuse()))) {
			// the document was wrong, the shoe file is not available
			pDoc->setShoeFilePresent(false);
			AfxMessageBox( "The program was not able to load the shoe models. "
				"The program will continue to operate normally, but without "
				"rendering shoes on the 3D models." );
			delete shoeMeshMaleLeft;
			delete shoeMeshMaleRight;
			delete shoeMeshFemaleLeft;
			delete shoeMeshFemaleRight;
		} else {
			// Figure out the bone indexes for the bones to bind the shoes to.
			map<string, boneInfo> boneInfoMap;
			list<string> boneNamesMale, boneNamesFemale;
			getBoneInfoFromModel(&maleMDP, boneInfoMap, boneNamesMale);
			getBoneInfoFromModel(&femaleMDP, boneInfoMap, boneNamesFemale);

			int leftAnkleIndexMale = distance(boneNamesMale.begin(), find(boneNamesMale.begin(), boneNamesMale.end(), "Left Ankle"));
			int rightAnkleIndexMale = distance(boneNamesMale.begin(), find(boneNamesMale.begin(), boneNamesMale.end(), "Right Ankle"));
			int leftAnkleIndexFemale = distance(boneNamesFemale.begin(), find(boneNamesFemale.begin(), boneNamesFemale.end(), "Left Ankle"));
			int rightAnkleIndexFemale = distance(boneNamesFemale.begin(), find(boneNamesFemale.begin(), boneNamesFemale.end(), "Right Ankle"));

			shoeMeshMaleLeft->ScaleMesh( -1.1, 1.0, 1.0 );
			shoeMeshMaleRight->ScaleMesh( 1.1, 1.0, 1.0 );
			shoeMeshFemaleLeft->ScaleMesh( -1.1, 1.0, 1.0 );
			shoeMeshFemaleRight->ScaleMesh( 1.1, 1.0, 1.0 );

			// Attach the shoes to the 3d mannequins.
			pVertex leftFootTransMale, rightFootTransMale;
			pVertex leftFootTransFemale, rightFootTransFemale;

			// Move shoes to proper location in world
			pVector3f_COPY(perihelion::g_origin3f, leftFootTransMale   );
			pVector3f_COPY(perihelion::g_origin3f, rightFootTransMale  );
			pVector3f_COPY(perihelion::g_origin3f, leftFootTransFemale );
			pVector3f_COPY(perihelion::g_origin3f, rightFootTransFemale);

			leftFootTransMale[0]    = -1.12f;
			rightFootTransMale[0]   =  1.12f;
			leftFootTransFemale[0]  = -1.13f;
			rightFootTransFemale[0] =  1.13f;
			leftFootTransMale[2] = rightFootTransMale[2] = leftFootTransFemale[2] = rightFootTransFemale[2] = 0.135f;

			perihelion::pQuat leftFootRotMale, rightFootRotMale;
			perihelion::pQuat leftFootRotFemale, rightFootRotFemale;

			perihelion::pVector3f zAxis = { 0.0, 0.0, 1.0 };

			pVector4f_COPY(perihelion::g_quatIdent, leftFootRotMale   );
			pVector4f_COPY(perihelion::g_quatIdent, rightFootRotMale  );
			pVector4f_COPY(perihelion::g_quatIdent, leftFootRotFemale );
			pVector4f_COPY(perihelion::g_quatIdent, rightFootRotFemale);

			perihelion::mdpQuatFromAngleAxis((GLfloat) M_PI / 6.0f , zAxis, leftFootRotMale   );
			perihelion::mdpQuatFromAngleAxis((GLfloat)-M_PI / 6.0f , zAxis, rightFootRotMale  );
			perihelion::mdpQuatFromAngleAxis((GLfloat) M_PI / 6.0f , zAxis, leftFootRotFemale );
			perihelion::mdpQuatFromAngleAxis((GLfloat)-M_PI / 6.0f , zAxis, rightFootRotFemale);

			// Attach the mesh to the model
			int maleLeftIndex = maleMDP.AddMesh( shoeMeshMaleLeft,      leftFootTransMale,    leftFootRotMale   );
			int maleRightIndex = maleMDP.AddMesh( shoeMeshMaleRight,     rightFootTransMale,   rightFootRotMale  );
			int femaleLeftIndex = femaleMDP.AddMesh( shoeMeshFemaleLeft,  leftFootTransFemale,  leftFootRotFemale );
			int femaleRightIndex = femaleMDP.AddMesh( shoeMeshFemaleRight, rightFootTransFemale, rightFootRotFemale);

			// Bind all the verts in the shoe mesh to the proper ankle bone in the model.
			maleMDP.AttachMeshToBone(leftAnkleIndexMale, maleLeftIndex);
			maleMDP.AttachMeshToBone(rightAnkleIndexMale, maleRightIndex);
			femaleMDP.AttachMeshToBone(leftAnkleIndexFemale, femaleLeftIndex);
			femaleMDP.AttachMeshToBone(rightAnkleIndexFemale, femaleRightIndex);

			float r = 117.0f / 255.0f;
			float g = 82.0f / 255.0f;
			float b = 51.0f / 255.0f;
			shoeMeshMaleLeft->UpdateVertexColors(r, g, b);
			shoeMeshMaleRight->UpdateVertexColors(r, g, b);
			shoeMeshFemaleLeft->UpdateVertexColors(r, g, b);
			shoeMeshFemaleRight->UpdateVertexColors(r, g, b);
		}
	}
}

GLOblique::~GLOblique(void) {
	if(bgPicture) bgPicture->Release();
    if(bgTexture) glDeleteTextures(1, &bgTexture);
}

void GLOblique::updateMDP() {
	// ----- Update OpenGL Hominoid -----

	// Make sure we're drawing the right Hominoid.
    int lSex = pDoc->GenderCode() + 1;
	if(lSex == 1) currentMDP = &maleMDP;
    if(lSex == 2) currentMDP = &femaleMDP;

    // Need to update title bars in case one or more hominoid models are not loaded
	//MainFramePtr()->Update_All_Title_Bars();

    // If the model is not inuse, it means there was an error loading
    // it from the file, and therefore cannot be displayed as is.

	if(currentMDP->Inuse())
    {
		const Anthropometry& anthropometry = pDoc->getTask().getAnthropometryRef();
		map<string, boneInfo> boneInfoMap;
		list<string> boneNames;
		getBoneInfoFromModel(currentMDP, boneInfoMap, boneNames);

        float height = anthropometry.GetHeight( MetricCns );
        float Weight = anthropometry.GetWeight( MetricCns );
        float defWeight;
        float heightOffset;

        // A guestimated metric for calculating approximate 
        if (lSex == 1)
        {
            // How far off from average height are we?
            heightOffset = (height - MALE_HEIGHT_50TH) / (MALE_HEIGHT_95TH - MALE_HEIGHT_5TH);

            // Calculate what the 50th percentile weight is likely to be for this height.
            // The relationship between height and weight is ^2, because height gives us the
            // axial component, and weight is an area-cross section.
            defWeight = heightOffset * (MALE_WEIGHT_95TH - MALE_WEIGHT_5TH);
            defWeight = defWeight < 0 ? -sqrtf(-(defWeight)) : sqrtf(defWeight);
            defWeight += MALE_WEIGHT_50TH;
        }
        else
        {
            // How far off from average height are we?
            heightOffset = (height - FEMALE_HEIGHT_50TH) / (FEMALE_HEIGHT_95TH - FEMALE_HEIGHT_5TH);

            // Calculate what the 50th percentile weight is likely to be for this height.
            // The relationship between height and weight is ^2, because height gives us the
            // axial component, and weight is an area-cross section.
            defWeight = heightOffset * (FEMALE_WEIGHT_95TH - FEMALE_WEIGHT_5TH);
            defWeight = defWeight < 0 ? -sqrtf(-(defWeight)) : sqrtf(defWeight);
            defWeight += FEMALE_WEIGHT_50TH;
        }

		Skeleton* skel = workingSkeleton();
		generateQuaternions(skel->GetAngles(), skel->GetAngle(A_PelvisFlexion), boneInfoMap, defWeight, Weight);

        //
        // Transfer bone data to the model.
        //
		currentMDP->LoadBoneAttributes(boneInfoMap, false);

        currentMDP->UpdateBones();
        currentMDP->UpdateVerts();
    }
}
void GLOblique::getBoneInfoFromModel(perihelion::MDP* model, map<string, boneInfo> &boneInfoMap, list<string> &boneNames)
{
	stringstream boneInfoFromModel;
	model->DumpBoneAttributes(boneInfoFromModel);

	//
	// Get the list of bones in the model.
	//

	while (boneInfoFromModel.good())
	{
		string boneName, dummy;
		perihelion::pQuat refQuat;
		perihelion::pQuat newLocalRotation;
		float weight, length, refweight, reflength;

		// Ignore bone scale info
		getline(boneInfoFromModel, boneName);
		boneInfoFromModel >> weight >> length >> refweight >> reflength;
		boneInfoFromModel.ignore(2, '\n');	
		getline(boneInfoFromModel, dummy);

		// Get the reference posture of this bone
		boneInfoFromModel >> refQuat[0];
		boneInfoFromModel >> refQuat[1];
		boneInfoFromModel >> refQuat[2];
		boneInfoFromModel >> refQuat[3];
		boneInfoFromModel.ignore(2, '\n');	

		// Make sure we have a valid quaternion - if its zero, make it an identity rotation
		if (refQuat[0] == 0 && refQuat[1] == 0 && refQuat[2] == 0 && refQuat[3] == 0)
		pVector4f_COPY(perihelion::g_quatIdent, refQuat);
		perihelion::pVectorNormalize4f	(refQuat, refQuat);

		// Make a list of the bone names
		boneNames.push_back(boneName);

		// Generate the info to send back to the model.
		pVector4f_COPY(refQuat, newLocalRotation);
		boneInfoMap[boneName] = boneInfo(weight, length, refweight, reflength, newLocalRotation);

		// peek to trigger eof() and !good() so we don't have an extra invalid entry in [""] of boneInfoMap
		boneInfoFromModel.peek();
	}
}
void GLOblique::generateQuaternions(JointAngles angle, float aPelvicTilt, std::map<std::string,boneInfo> &bones, float defWeight, float Weight) {

	Skeleton* skel = workingSkeleton();
	
	const Factors& factors = pDoc->GetFactors();

	perihelion::pVector3d XAXIS = { 1.0, 0.0, 0.0 };
	perihelion::pVector3d YAXIS = { 0.0, 1.0, 0.0 };
	perihelion::pVector3d ZAXIS = { 0.0, 0.0, 1.0 };

	// This is the standard way to get the height of the hominoid 
	// Please use this standard method to get the weight in any future references
	const Anthropometry& anthropometry = pDoc->getTask().getAnthropometryRef();
    double height = anthropometry.GetHeight( MetricCns );

	// --- Shoulder adjustment --- //
	// move shoulder up the spine by some constant
	// calculate lengths for clavicle & upper arm
	// calculate clavicle and shoulder angles
	const double shoulderDisplacement = shouldDisp;
	Vector3 C7T1 = skel->getJoint(JT_C7T1, true);
	Vector3 spine = C7T1 - skel->getJoint(JT_L5S1_TEMP, true);
	spine.normalize();
	Vector3 right = -cross(spine, skel->getJoint(JT_SCJ, true) - C7T1);
	right.normalize();
	Vector3 back = cross(right, spine);
	back.normalize();

	// --- re-calculate length for different body parts --- //
	double hipwidth = (skel->getJoint(JT_RHIP, true)-skel->getJoint(JT_LHIP, true)).length()/height;
	Vector3 rShoulderAdj = skel->getJoint(JT_RSHOULDER, true) + spine * shoulderDisplacement;
	Vector3 lShoulderAdj = skel->getJoint(JT_LSHOULDER, true) + spine * shoulderDisplacement;
	double rClavLength = (rShoulderAdj - (skel->getJoint(JT_RSHOULDER, true)+skel->getJoint(JT_LSHOULDER, true))/2).length() / height;
	double lClavLength = (lShoulderAdj - (skel->getJoint(JT_RSHOULDER, true)+skel->getJoint(JT_LSHOULDER, true))/2).length() / height;
	double rUpperArmLength = (rShoulderAdj - skel->getJoint(JT_RELBOW, true)).length() / height;
	double lUpperArmLength = (lShoulderAdj - skel->getJoint(JT_LELBOW, true)).length() / height;
	double rLowerArmLength = (skel->getJoint(JT_RELBOW, true) - skel->getJoint(JT_RWRIST, true)).length() / height;
	double lLowerArmLength = (skel->getJoint(JT_LELBOW, true) - skel->getJoint(JT_LWRIST, true)).length() / height;
	double rUpperLegLength = (skel->getJoint(JT_RHIP, true) - skel->getJoint(JT_RKNEE, true)).length() / height;
	double lUpperLegLength = (skel->getJoint(JT_LHIP, true) - skel->getJoint(JT_LKNEE, true)).length() / height;
	double rLowerLegLength = (skel->getJoint(JT_RKNEE, true) - skel->getJoint(JT_RANKLE, true)).length() / height;
	double lLowerLegLength = (skel->getJoint(JT_LKNEE, true) - skel->getJoint(JT_LANKLE, true)).length() / height;
	double rAnkleLength = (skel->getJoint(JT_RHEEL, true) - skel->getJoint(JT_RBALL, true)).length() / height
						 - (skel->getJoint(JT_RHEEL, true) - skel->getJoint(JT_RANKLE, true)).length() / height;
	double lAnkleLength = (skel->getJoint(JT_LHEEL, true) - skel->getJoint(JT_LBALL, true)).length() / height
						 - (skel->getJoint(JT_LHEEL, true) - skel->getJoint(JT_LANKLE, true)).length() / height;
	double torsoLength = (skel->getJoint(JT_C7T1, true) - skel->getJoint(JT_L5S1_TEMP, true)).length() / height;
	double upperNeckLength = (skel->getJoint(JT_C1, true) - skel->getJoint(JT_C3C4, true)).length() / height;
	double lowerNeckLength = (skel->getJoint(JT_C3C4, true) - skel->getJoint(JT_C7T1, true)).length() / height;

	//
	// Use the glHom's anthropometry by default
	//bones["Right Hip"].length       = bones["Right Hip"].refLength;
	//bones["Left Hip"].length        = bones["Left Hip"].refLength;
	bones["Right Hip"].length       = hipwidth/1.3;
	bones["Left Hip"].length        = hipwidth/1.3;
	bones["Head"].length			= abs(factors.default_mLink_C1Tragion - (factors.mLink_C1Tragion - factors.default_mLink_C1Tragion) - factors.mLink_HeadRadius); // = factors.mLink_HeadRadius-factors.mLink_C1Tragion;
	bones["Upper Neck"].length		= upperNeckLength;		// = factors.mLink_UpperneckLength;
	bones["Lower Neck"].length		= lowerNeckLength;		// = factors.mLink_LowerneckLength;
	bones["PelvisLength"].length	= factors.mLink_HipToL5S1;
	bones["Torso"].length			= torsoLength;			// = factors.mLink_L5S1ToC7T1;
	bones["Left Clavicle"].length	= lClavLength;
	bones["Right Clavicle"].length	= rClavLength;

	bones["Left Upper Arm"].length	= lUpperArmLength;
	bones["Left Lower Arm"].length	= lLowerArmLength;		// = factors.mLink_Forearm;
	bones["Left Upper Leg"].length	= lUpperLegLength;		// = factors.mLink_UpperLeg;
	bones["Left Lower Leg"].length	= lLowerLegLength;		// = factors.mLink_LowerLeg;

	bones["Right Upper Arm"].length	= rUpperArmLength;
	bones["Right Lower Arm"].length = rLowerArmLength;		// = factors.mLink_Forearm;
	bones["Right Upper Leg"].length = rUpperLegLength;		// = factors.mLink_UpperLeg;
	bones["Right Lower Leg"].length = rLowerLegLength;		// = factors.mLink_LowerLeg;

	bones["Right Ankle"].length		= rAnkleLength * 0.8;		// the 0.8 coefficient is added to ensure a fancy appearance
	bones["Left Ankle"].length		= lAnkleLength * 0.8;

	//
	// Dont modify the hip lengths! The glHom linkage is slightly different
	// than the 3dsspp's scaling.  The glHom should be re-linked if the hip
	// anthropometry changes significantly.
	//
	bones["C7T1 to SCJ"].length     = factors.mLink_C7T1toSCJ;
	bones["Hip Width"].length       = factors.mLink_HipWidth * 0.5F;
	bones["L5S1 to IT"].length      = factors.mLink_L5S1ToIT;


	/* Below are legacy code that calculate the lengths of the different body parts */
	//bones["Left Clavicle"].length    = factors.mLink_ShoulderWidth * 0.5F ;
	//bones["Left Clavicle"].length = (skel->getJoint(JT_LSHOULDER, true) - skel->getJoint(JT_C7T1, true)).length() * factors.mLink_LowerLeg
	//	/ (skel->getJoint(JT_RANKLE, true) - skel->getJoint(JT_RKNEE, true)).length();
	//bones["Left Upper Arm"].length   = factors.mLink_Upperarm             ;
	//bones["Right Clavicle"].length   = factors.mLink_ShoulderWidth * 0.5F ;
	//bones["Right Clavicle"].length = (skel->getJoint(JT_RSHOULDER, true) - skel->getJoint(JT_C7T1, true)).length() * factors.mLink_LowerLeg
	//	/ (skel->getJoint(JT_RANKLE, true) - skel->getJoint(JT_RKNEE, true)).length();
	//bones["Right Upper Arm"].length  = factors.mLink_Upperarm             ;
	//bones["Right Ankle"].length = factors.mLink_HeelToToe - factors.mLink_HeelLength;
	//bones["Left Ankle"].length = factors.mLink_HeelToToe - factors.mLink_HeelLength;


	

	Vector3 clavVect = rShoulderAdj - C7T1;
	double rClavV = 90 - degreesBetween(spine, clavVect);
	clavVect -= spine * (spine * clavVect);
	double rClavH = degreesBetween(clavVect, right);
	if(clavVect * back > 0) rClavH *= -1;
	clavVect = lShoulderAdj - C7T1;
	double lClavV = 90 - degreesBetween(spine, clavVect);
	clavVect -= spine * (spine * clavVect);
	double lClavH = degreesBetween(clavVect, -right);
	if(clavVect * back > 0) lClavH *= -1;

	double rShoulderV = degreesBetween(C7T1 - skel->getJoint(JT_L5S1_TEMP,true), rShoulderAdj - skel->getJoint(JT_RELBOW, true));
	double lShoulderV = degreesBetween(C7T1 - skel->getJoint(JT_L5S1_TEMP,true), lShoulderAdj - skel->getJoint(JT_LELBOW, true));

	Vector3 joints[NUM_JOINTS];
	skel->getJoints(joints, true);
	joints[JT_RSHOULDER] = rShoulderAdj;
	joints[JT_LSHOULDER] = lShoulderAdj;
	JointAngles ja = skel->GetAngles();
	double extraAngles[NUM_EXTRA_ANGLES];
	skel->getExtraAngles(extraAngles);
	extraAngles[EA_ShoulderRV] = rShoulderV;
	extraAngles[EA_ShoulderLV] = lShoulderV;
	Vector3 sdv[NUM_STRENGTH_HINGES];
	skel->getStrengthDirectionVectors(sdv);
	Analysis analysis;
	double rShoulderH = analysis.ComputeShoulderRH(ja, joints, sdv, extraAngles);
	if((extraAngles[EA_ShoulderRV] < 1) || (extraAngles[EA_ShoulderRV] > 179)) {
		rShoulderH = 90;
	}
	double lShoulderH = analysis.ComputeShoulderLH(ja, joints, sdv, extraAngles);
		if((extraAngles[EA_ShoulderLV] < 1) || (extraAngles[EA_ShoulderLV] > 179)) {
		lShoulderH = 90;
	}

	// make all refWeights defWeight by default
	for(std::map<std::string, boneInfo>::iterator it = bones.begin(); it != bones.end(); ++it) {
		(*it).second.refWeight = defWeight;
	}
	bones["Torso"].refWeight           = defWeight;
	bones["Head"].refWeight			   = defWeight;
	bones["Left Clavicle"].refWeight   = defWeight;
	bones["Right Clavicle"].refWeight  = defWeight;
	bones["Right Hip"].refWeight       = defWeight;
	bones["Left Hip"].refWeight        = defWeight;
	bones["Left Upper Arm"].refWeight  = defWeight;
	bones["Left Lower Arm"].refWeight  = defWeight;
	bones["Left Upper Leg"].refWeight  = defWeight;
	bones["Left Lower Leg"].refWeight  = defWeight;
	bones["Right Upper Arm"].refWeight = defWeight;
	bones["Right Lower Arm"].refWeight = defWeight;
	bones["Right Upper Leg"].refWeight = defWeight;
	bones["Right Lower Leg"].refWeight = defWeight;
	bones["Left Wrist"].refWeight		= defWeight;
	bones["Right Wrist"].refWeight		= defWeight;
	bones["Left Ankle"].refWeight		= defWeight;
	bones["Right Ankle"].refWeight		= defWeight;
	bones["Upper Neck"].refWeight		= defWeight;
	bones["Lower Neck"].refWeight		= defWeight;

	//
	// The actual scaling outwards of the mesh will be propotional
	// to the square-root of the difference of the reference weight 
	// and the actual weight.  This is because the scaling is 
	// acted upon the radius of the cross-section.
	//
	float scaling = (Weight - defWeight);
	scaling = scaling < 0 ? -sqrtf(-(scaling)) : sqrtf(scaling);
	scaling *= 2.0;			// exaggerate the scaling for 'artistic' purposes
	scaling += defWeight;

	LinkWeights standardWeights = anthropometry.getRefWeights();
	LinkWeights newWeights = anthropometry.getNewWeights();
	int lSex = pDoc->GenderCode();
		double shrinkFactor;
	if (lSex == 0)
	{shrinkFactor = 1.00;}
	else
	{shrinkFactor = 1.10;}
	for(std::map<std::string, boneInfo>::iterator it = bones.begin(); it != bones.end(); ++it) {
		(*it).second.weight = scaling;
	}
	
	// update the weights
	if(standardWeights.Hand > 0) {		// standard weight has been successfully set
		bones["Torso"].weight           = scaling * sqrt(newWeights.TorsoAndHead / standardWeights.TorsoAndHead) * shrinkFactor;
		bones["Head"].weight			= scaling;
		bones["Left Clavicle"].weight   = scaling* sqrt(newWeights.TorsoAndHead / standardWeights.TorsoAndHead) * shrinkFactor;
		bones["Right Clavicle"].weight  = scaling* sqrt(newWeights.TorsoAndHead / standardWeights.TorsoAndHead) * shrinkFactor;
		bones["Right Hip"].weight       = scaling* sqrt(newWeights.TorsoAndHead / standardWeights.TorsoAndHead) * shrinkFactor;
		bones["Left Hip"].weight        = scaling* sqrt(newWeights.TorsoAndHead / standardWeights.TorsoAndHead) * shrinkFactor;
		bones["Left Upper Arm"].weight  = scaling * sqrt(newWeights.UpperArm / standardWeights.UpperArm) * shrinkFactor;
		bones["Left Lower Arm"].weight  = scaling * sqrt(newWeights.Forearm / standardWeights.Forearm) * shrinkFactor;
		bones["Left Upper Leg"].weight  = scaling * sqrt(newWeights.UpperLeg / standardWeights.UpperLeg) * shrinkFactor;
		bones["Left Lower Leg"].weight  = scaling * sqrt(newWeights.LowerLeg / standardWeights.LowerLeg) * shrinkFactor;
		bones["Right Upper Arm"].weight = scaling * sqrt(newWeights.UpperArmR / standardWeights.UpperArmR) * shrinkFactor;
		bones["Right Lower Arm"].weight = scaling * sqrt(newWeights.ForearmR / standardWeights.ForearmR) * shrinkFactor;
		bones["Right Upper Leg"].weight = scaling * sqrt(newWeights.UpperLegR / standardWeights.UpperLegR) * shrinkFactor;
		bones["Right Lower Leg"].weight = scaling * sqrt(newWeights.LowerLegR / standardWeights.LowerLegR) * shrinkFactor;
		// bones["Left Wrist"].weight		= scaling;
		// bones["Right Wrist"].weight		= scaling;
		bones["Left Ankle"].weight		= scaling * sqrt(newWeights.Foot / standardWeights.Foot) * shrinkFactor;
		bones["Right Ankle"].weight		= scaling * sqrt(newWeights.FootR / standardWeights.FootR) * shrinkFactor;
		bones["Upper Neck"].weight		= scaling * sqrt(newWeights.UpperNeck / standardWeights.UpperNeck) * shrinkFactor;
		bones["Lower Neck"].weight		= scaling * sqrt(newWeights.LowerNeck / standardWeights.LowerNeck) * shrinkFactor;
	} 
	/*
	else {	// standard weights haven't been successfully set
		// set all weights to scaling by default
		for(std::map<std::string, boneInfo>::iterator it = bones.begin(); it != bones.end(); ++it) {
			(*it).second.weight = scaling;
		}
	}
	*/


	//double PelvAng = CalculatePelvicAngle(aPelvicTilt, angle);
	double PelvAng = skel->getExtraAngle(EA_PelvisFlexion);

	/***** Calculate torso angle, L4 to T4 with respect to horizontal.*/
	double TorsoFlexion = skel->getExtraAngle(EA_TrunkFlexion);

	// ================================================================
	// Correct angles, as they're global angles, and we need local.
	// ================================================================
	TorsoFlexion -= 90;

	// =================================================================
	//  Begin creating quaternions for local rotation of each link.
	// =================================================================


	CQuaternion torsoReference(bones["Torso"].q, "Torso Reference");
	CQuaternion RUAref(bones["Right Upper Arm"].q, "Right Upper Arm Reference");
	CQuaternion LUAref(bones["Left Upper Arm"].q , "Left Upper Arm Reference" );
	CQuaternion RLAref(bones["Right Lower Arm"].q, "Right Lower Arm Reference");
	CQuaternion LLAref(bones["Left Lower Arm"].q , "Left Lower Arm Reference" );

	CQuaternion rootLocal =	CQuaternion(ZAXIS, angle[A_PelvisAxialRotation] * DTOR, "pelvis axial") *
							CQuaternion(YAXIS, angle[A_PelvisLateralBending] * DTOR, "pelvis lateral bending") *
							CQuaternion(XAXIS, -PelvAng * DTOR, "pelvis flex");

	CQuaternion torsoLocal = CQuaternion(ZAXIS, angle[A_TrunkAxialRotation]  * DTOR, "Torso Axial Rotation" ) *
							 CQuaternion(YAXIS, angle[A_TrunkLateralBending] * DTOR, "Torso Lateral Bending") *
							 CQuaternion(XAXIS, TorsoFlexion  * DTOR               , "Torso Flexion"        );

	CQuaternion lowerneckLocal =	CQuaternion(ZAXIS, angle[A_C4C7AxialRotation] * DTOR, "LowerNeck Axial Rotation" ) *
							CQuaternion(YAXIS, angle[A_C4C7LateralBending] * DTOR, "LowerNeck Lateral Bending" ) *
							CQuaternion(XAXIS, (angle[A_C4C7Flexion]-90) * DTOR, "LowerNeck Flexion" );

	// Add:
	CQuaternion upperneckLocal = CQuaternion(ZAXIS, angle[A_C1C3_AxialRotation] * DTOR, "Upper Neck Axial Rotation" ) *
								 CQuaternion(YAXIS, angle[A_C1C3_LateralBending] * DTOR, "Upper Neck Lateral Bending" ) *
								 CQuaternion(XAXIS, (angle[A_C1C3_Flexion]-90) * DTOR, "Upper Neck Flexion" );

	// Add C1
	/*
	CQuaternion C1Local = CQuaternion(ZAXIS, angle[A_C1_AxialRotation] * DTOR, "C1 Axial Rotation" ) *
								 CQuaternion(YAXIS, angle[A_C1_LateralBending] * DTOR, "C1 Lateral Bending" ) *
								 CQuaternion(XAXIS, (angle[A_C1_Flexion] - 90 + 16	// add 16 to make it look right on the stick figure and the hominoid
													) * DTOR, "C1 Flexion" );		// but note that we are not making any changes to the actual angles that 
																					// we uses for calculations

	*/
	CQuaternion C1Local = CQuaternion(ZAXIS, angle[A_C1_AxialRotation] * DTOR, "C1 Axial Rotation" ) *
								 CQuaternion(YAXIS, angle[A_C1_LateralBending] * DTOR, "C1 Lateral Bending" ) *
								 CQuaternion(XAXIS, (pDoc->GetFactors().mAngle_Head_Flexion - 90 + 16	// add 16 to make it look right on the stick figure and the hominoid
													) * DTOR, "C1 Flexion" );		// but note that we are not making any changes to the actual angles that 
																					// we uses for calculations
																					// also note that we are using the angle from factors.cpp

	// Right Arm
	//int sign = skel->getExtraAngle(EA_ShoulderRH) < 0 ? -1 : 1;
	int sign = rShoulderH < 0 ? -1 : 1;
	/*double fakeClavicleV, fakeClavicleH;
	Vector3 spine = skel->getJoint(JT_C7T1, true) - skel->getJoint(JT_L5S1_TEMP, true);
	spine.normalize();
	Vector3 right = -cross(spine, skel->getJoint(JT_SCJ, true) - skel->getJoint(JT_C7T1, true));
	Vector3 back = cross(right, spine);
	Vector3 fakeClav = skel->getJoint(JT_RSHOULDER, true) - skel->getJoint(JT_C7T1, true);
	fakeClavicleV = 90 - degreesBetween(spine, fakeClav);
	//fakeClavicleV = VerticalAngle(skel->getJoint(JT_RSHOULDER, true) - skel->getJoint(JT_C7T1, true));
	fakeClav -= spine * (spine * fakeClav);
	fakeClavicleH = degreesBetween(fakeClav, right);
	if(fakeClav * back > 0) fakeClavicleH *= -1;*/
	//fakeClavicleH = RightHorizontalAngle(skel->getJoint(JT_RSHOULDER, true) - skel->getJoint(JT_C7T1, true));
	/*CQuaternion RClavicle = CQuaternion(YAXIS, -angle[A_ClavicleRV] * DTOR, "v clav") *
							CQuaternion(ZAXIS, angle[A_ClavicleRH] * DTOR, "h clav");*/
	/*CQuaternion RClavicle = CQuaternion(YAXIS, -fakeClavicleV * DTOR, "v clav") *
							CQuaternion(ZAXIS, fakeClavicleH * DTOR, "h clav");*/
	CQuaternion RClavicle = CQuaternion(YAXIS, -rClavV * DTOR, "v clav") *
							CQuaternion(ZAXIS, rClavH * DTOR, "h clav");
	/*CQuaternion RUpper =	CQuaternion(ZAXIS, skel->getExtraAngle(EA_HumerusRRot) * DTOR, "hrot") *
							CQuaternion(XAXIS, sign*skel->getExtraAngle(EA_ShoulderRV) * DTOR, "RSV") *
							CQuaternion(ZAXIS, (skel->getExtraAngle(EA_ShoulderRH) - sign*90) * DTOR, "RSH");*/
	CQuaternion RUpper =	CQuaternion(ZAXIS, skel->getExtraAngle(EA_HumerusRRot) * DTOR, "hrot") *
							CQuaternion(XAXIS, sign*rShoulderV * DTOR, "RSV") *
							CQuaternion(ZAXIS, (rShoulderH - sign*90) * DTOR, "RSH");
	CQuaternion RLower =	CQuaternion(ZAXIS, -skel->getExtraAngle(EA_ForearmRRot) * DTOR, "sup") *
							CQuaternion(XAXIS, (180 - skel->getExtraAngle(EA_ElbowRInc)) * DTOR, "elbow");
	CQuaternion RHand  =	CQuaternion(XAXIS, (-skel->getExtraAngle(EA_HandRDeviation) - 10	// subtract 10 more degrees to make the hominoid look good
												) * DTOR, "deviation") *						// but we are touching the actual data used in calculations
							CQuaternion(YAXIS, -skel->getExtraAngle(EA_HandRFlexion) * DTOR, "flexion");


	//sign = skel->getExtraAngle(EA_ShoulderLH) < 0 ? -1 : 1;
	sign = lShoulderH < 0 ? -1 : 1;
	/*fakeClav = skel->getJoint(JT_LSHOULDER, true) - skel->getJoint(JT_C7T1, true);
	fakeClavicleV = 90 - degreesBetween(spine, fakeClav);
	//fakeClavicleV = VerticalAngle(skel->getJoint(JT_LSHOULDER, true) - skel->getJoint(JT_C7T1, true));
	fakeClav -= spine * (spine * fakeClav);
	fakeClavicleH = degreesBetween(fakeClav, -right);
	if(fakeClav * back > 0) fakeClavicleH *= -1;*/
	//fakeClavicleH = LeftHorizontalAngle(skel->getJoint(JT_LSHOULDER, true) - skel->getJoint(JT_C7T1, true));
	/*CQuaternion LClavicle = CQuaternion(YAXIS, angle[A_ClavicleLV] * DTOR, "v clav") *
							CQuaternion(ZAXIS, -angle[A_ClavicleLH] * DTOR, "h clav");*/
	/*CQuaternion LClavicle = CQuaternion(YAXIS, fakeClavicleV * DTOR, "v clav") *
							CQuaternion(ZAXIS, -fakeClavicleH * DTOR, "h clav");*/
	CQuaternion LClavicle = CQuaternion(YAXIS, lClavV * DTOR, "v clav") *
							CQuaternion(ZAXIS, -lClavH * DTOR, "h clav");
	/*CQuaternion LUpper =	CQuaternion(ZAXIS, -skel->getExtraAngle(EA_HumerusLRot) * DTOR, "hrot") *
							CQuaternion(XAXIS, sign*skel->getExtraAngle(EA_ShoulderLV) * DTOR, "SLV") *
							CQuaternion(ZAXIS, -(skel->getExtraAngle(EA_ShoulderLH)-sign*90) * DTOR, "SLH");*/
	CQuaternion LUpper =	CQuaternion(ZAXIS, -skel->getExtraAngle(EA_HumerusLRot) * DTOR, "hrot") *
							CQuaternion(XAXIS, sign*lShoulderV * DTOR, "SLV") *
							CQuaternion(ZAXIS, -(lShoulderH - sign*90) * DTOR, "SLH");
	CQuaternion LLower =	CQuaternion(ZAXIS, skel->getExtraAngle(EA_ForearmLRot) * DTOR, "sup") *
							CQuaternion(XAXIS, (180 - skel->getExtraAngle(EA_ElbowLInc)) * DTOR, "elbow");
	CQuaternion LHand  =	CQuaternion(XAXIS, (-skel->getExtraAngle(EA_HandLDeviation) - 10	// subtract 10 more degrees to make the hominoid look good
												) * DTOR, "deviation") *						// but we are touching the actual data used in calculations
							CQuaternion(YAXIS, skel->getExtraAngle(EA_HandLFlexion) * DTOR, "flexion");

							

	// Get Reference quaternions
	CQuaternion RUAlocal = RUpper;
	CQuaternion LUAlocal = LUpper;
	CQuaternion RLAlocal = RLower;
	CQuaternion LLAlocal = LLower;
	CQuaternion RHlocal  = RHand;
	CQuaternion LHlocal  = LHand;

	CQuaternion torsoBase =	torsoReference;
	CQuaternion RUABase   =          RUAref;
	CQuaternion LUABase   =          LUAref;
	CQuaternion RLABase   = RLAref * RUAref;
	CQuaternion LLABase   = LLAref * LUAref;
	CQuaternion RHBase	  = RLAref * RUAref;
	CQuaternion LHBase	  = LLAref * LUAref;

	CQuaternion torsoComposite = torsoBase * torsoLocal * rootLocal.GetInverse();
	CQuaternion lowerneckComposite  = lowerneckLocal;
	CQuaternion upperneckComposite = upperneckLocal;
	CQuaternion headComposite = C1Local;
	CQuaternion RCComposite	   = RClavicle;
	CQuaternion LCComposite    = LClavicle;
	CQuaternion RUAComposite   = RUABase * RUAlocal * RCComposite.GetInverse();
	CQuaternion LUAComposite   = LUABase * LUAlocal * LCComposite.GetInverse();
	// Lower arm is put into default position after RLAref AND RUAref, so we do both first,
	// then the local rotation, then inverse RUAref, so when it gets multiplied buy RUAref in the mdp
	// code they will cancel. yes, this is annoying.
	CQuaternion RLAComposite   = RLABase * RLAlocal * RUABase.GetInverse();
	CQuaternion LLAComposite   = LLABase * LLAlocal * LUABase.GetInverse();
	CQuaternion RHComposite	   = RHBase * RHlocal * RLABase.GetInverse();
	CQuaternion LHComposite	   = LHBase * LHlocal * LLABase.GetInverse();

	pVector4f_COPY(rootLocal.q, bones["Root"].q);
	pVector4f_COPY(torsoComposite.q, bones["Torso"].q);
	pVector4f_COPY(lowerneckComposite.q, bones["Lower Neck"].q);
	pVector4f_COPY(upperneckComposite.q, bones["Upper Neck"].q);
	pVector4f_COPY(headComposite.q, bones["Head"].q);
	pVector4f_COPY(RCComposite.q, bones["Right Clavicle"].q);
	pVector4f_COPY(LCComposite.q, bones["Left Clavicle"].q);
	pVector4f_COPY(RUAComposite.q, bones["Right Upper Arm"].q);
	pVector4f_COPY(LUAComposite.q, bones["Left Upper Arm"].q ); 
	pVector4f_COPY(RLAComposite.q, bones["Right Lower Arm"].q);
	pVector4f_COPY(LLAComposite.q, bones["Left Lower Arm"].q );
	pVector4f_COPY(RHComposite.q, bones["Right Wrist"].q );
	pVector4f_COPY(LHComposite.q, bones["Left Wrist"].q );

	// TODO testing applying hand angles to all metacarpals to reinstate wrist angles on new hominoid
	pVector4f_COPY(RHComposite.q, bones["Right thumb carpal"].q);
	pVector4f_COPY(RHComposite.q, bones["Right index carpal"].q);
	pVector4f_COPY(RHComposite.q, bones["Right long carpal"].q);
	pVector4f_COPY(RHComposite.q, bones["Right ring carpal"].q);
	pVector4f_COPY(RHComposite.q, bones["Right little carpal"].q);
	pVector4f_COPY(LHComposite.q, bones["Left thumb carpal"].q);
	pVector4f_COPY(LHComposite.q, bones["Left index carpal"].q);
	pVector4f_COPY(LHComposite.q, bones["Left long carpal"].q);
	pVector4f_COPY(LHComposite.q, bones["Left ring carpal"].q);
	pVector4f_COPY(LHComposite.q, bones["Left little carpal"].q);

	/// Get the reference quaternions for all the bones in the lower body
	CQuaternion RULref(bones["Right Upper Leg"].q, "Right Upper Leg Reference");
	CQuaternion LULref(bones["Left Upper Leg"].q , "Left Upper Leg Reference" );
	CQuaternion RLLref(bones["Right Lower Leg"].q, "Right Lower Leg Reference");
	CQuaternion LLLref(bones["Left Lower Leg"].q , "Left Lower Leg Reference" );
	CQuaternion RAref(bones["Right Ankle"].q, "Right Ankle Reference");
	CQuaternion LAref(bones["Left Ankle"].q , "Left Ankle Reference" );

	// Calculate the compounded reference posture for each lower body joint.
	CQuaternion RULBase   =                  RULref;
	CQuaternion LULBase   =                  LULref;
	CQuaternion RLLBase   =         RLLref * RULref;
	CQuaternion LLLBase   =         LLLref * LULref;
	CQuaternion RABase    = RAref * RLLref * RULref;
	CQuaternion LABase    = LAref * LLLref * LULref;

	// Calculate the local rotations for each leg joint
	CQuaternion HipLocal =	CQuaternion(ZAXIS, angle[A_PelvisAxialRotation] * DTOR, "hip axial") *
							CQuaternion(YAXIS, angle[A_PelvisLateralBending] * DTOR, "PLB") *
							CQuaternion(XAXIS, -PelvAng * DTOR, "Forward Pelvis Rotation");
	sign = (skel->getExtraAngle(EA_HipRH) < 0) ? -1 : 1;
	CQuaternion RULlocal =	CQuaternion(ZAXIS, skel->getExtraAngle(EA_FemurRRot) * DTOR, "femur") *
							CQuaternion(XAXIS, sign*(skel->getExtraAngle(EA_HipRV)) * DTOR, "hip v") *
							CQuaternion(ZAXIS, -(skel->getExtraAngle(EA_HipRH)-sign*90) * DTOR, "hip h");
	CQuaternion RLLlocal =	CQuaternion(ZAXIS, -skel->getExtraAngle(EA_LowerlegRRot) * DTOR, "lower rot") *
							CQuaternion(XAXIS, -(180-skel->getExtraAngle(EA_KneeRInc)) * DTOR, "knee");
	sign = (skel->getExtraAngle(EA_HipLH) < 0) ? -1 : 1;
	CQuaternion LULlocal =	CQuaternion(ZAXIS, -skel->getExtraAngle(EA_FemurLRot) * DTOR, "femur") *
							CQuaternion(XAXIS, sign*(skel->getExtraAngle(EA_HipLV)) * DTOR, "hip v") *
							CQuaternion(ZAXIS, -(skel->getExtraAngle(EA_HipLH)-sign*90) * DTOR, "hip h");
	CQuaternion LLLlocal =	CQuaternion(ZAXIS, skel->getExtraAngle(EA_LowerlegLRot) * DTOR, "lower rot") *
							CQuaternion(XAXIS, -(180-skel->getExtraAngle(EA_KneeLInc)) * DTOR, "knee");

	/*CQuaternion RAlocal =	CQuaternion(XAXIS, (90-skel->getExtraAngle(EA_AnkleRInc)) * DTOR, "ankle inc");
	CQuaternion LAlocal =	CQuaternion(XAXIS, (90-skel->getExtraAngle(EA_AnkleLInc)) * DTOR, "ankle inc");*/

	// Added incurred lateral bending to get the foot straight up and down, like in skeleton views
	CQuaternion RAlocal =	CQuaternion(YAXIS, skel->getRAnkleLateralBending() * DTOR, "ankle lateral bending") *
							CQuaternion(XAXIS, (90-skel->getExtraAngle(EA_AnkleRInc)) * DTOR, "ankle inc");
	CQuaternion LAlocal =	CQuaternion(YAXIS, skel->getLAnkleLateralBending() * DTOR, "ankle lateral bending") *
							CQuaternion(XAXIS, (90-skel->getExtraAngle(EA_AnkleLInc)) * DTOR, "ankle inc");

	// Every rotation is the combination of the base rotation which gets it to the proper reference orientation,
	// the local angles provided by 3dsspp, and the INVERSE of it's parent rotation [as the model does nested
	// rotations, and we want to undo the nesting].
	CQuaternion HipComposite = HipLocal * rootLocal.GetInverse();
	CQuaternion RULComposite = RULBase * RULlocal;
	CQuaternion LULComposite = LULBase * LULlocal;
	CQuaternion RLLComposite = RLLBase * RLLlocal * RULBase.GetInverse();
	CQuaternion LLLComposite = LLLBase * LLLlocal * LULBase.GetInverse();
	CQuaternion RAComposite = RABase * RAlocal * RLLBase.GetInverse();
	CQuaternion LAComposite = LABase * LAlocal * LLLBase.GetInverse();

	// Save the rotations.
	pVector4f_COPY(HipComposite.q, bones["Right Hip"].q);
	pVector4f_COPY(HipComposite.q, bones["Left Hip"].q);
	pVector4f_COPY(RULComposite.q, bones["Right Upper Leg"].q);
	pVector4f_COPY(LULComposite.q, bones["Left Upper Leg"].q ); 
	pVector4f_COPY(RLLComposite.q, bones["Right Lower Leg"].q);
	pVector4f_COPY(LLLComposite.q, bones["Left Lower Leg"].q );
	pVector4f_COPY(RAComposite.q , bones["Right Ankle"].q    );
	pVector4f_COPY(LAComposite.q , bones["Left Ankle"].q     );

	// to see hominoid with no rotations
	//for_each(bones.begin(), bones.end(), ptr_fun(setQuatIDInMap));
}

void GLOblique::draw(void) {

	
	// set the viewport
	//CRect client;
	//GetClientRect(&client);
	glViewport(viewingRect.left,viewingRect.top,viewingRect.Width(), viewingRect.Height());//client.Width(),client.Height());
	
	// clear the background and depth buffer
	// glClearColor(backgroundColor[0]/255.0f, backgroundColor[1]/255.0f, backgroundColor[2]/255.0f, 1.0f);
	// clear by drawing a polygon, because sometimes we are only rendering to part of the context,
	// but glClear clears all the pixels
	glColor4f(backgroundColor[0]/255.0f, backgroundColor[1]/255.0f, backgroundColor[2]/255.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho(0,1,0,1,-1,1);
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(1,0);
	glVertex2f(1,1);
	glVertex2f(0,1);
	glEnd();
	glPopMatrix();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);//QUAL);
	glEnable(GL_NORMALIZE);
	glLineWidth(1.0f);


	glClearDepth(100.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Put lights into the scene
	float lightIntensity;
	for(int i = 0; i < 9; ++i)
		glDisable(GL_LIGHT0 + i);
	if(pDoc->UsingNewLightScheme()) {
		lightIntensity = 1.0f * pDoc->GetLightIntensity() / 100.0f;

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);

		float pos1[] = {200,0,0,0};
		float pos2[] = {0,200,0,0};
		float pos3[] = {0,0,200,0};
		glLightfv(GL_LIGHT0,GL_POSITION,pos1);
		glLightfv(GL_LIGHT1,GL_POSITION,pos2);
		glLightfv(GL_LIGHT2,GL_POSITION,pos3);
	} else {
		lightIntensity = 1.6f * pDoc->GetLightIntensity() / 100.0f;

		glEnable(GL_LIGHT0);

		float position[] = {(pDoc->GetOblLightTypeLR() - 1)*200.0f,(pDoc->GetOblLightTypeTB() - 1)*-200.0f,
							(pDoc->GetOblLightTypeFB() - 1)*-200.0f,0.0f};
		glLightfv(GL_LIGHT0,GL_POSITION,position);
	}

	for(int i = 0; i < 3; ++i) {
		float amb[] = {0.1f,0.1f,0.1f,0.0f};
		float diff[] = {lightIntensity,lightIntensity,lightIntensity,0};
		glLightfv(GL_LIGHT0 + i,GL_AMBIENT,amb);
		glLightfv(GL_LIGHT0 + i,GL_DIFFUSE,diff);
	}

	pVector4f mat_specular = {0.3f,0.3f,0.3f,0.3f};
	GLfloat mat_shininess[] = {50.0f};

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	// render a background image if necessary
	if(bgActive) {
		// since gl context must be active to reassign textures, most of the background updating is done here
		if(bgPostUpdate) {
			bgPostUpdate = false;
			updateBackground();
		}
		drawBackground();
	}
	// Bail out if the hominoid model has not been successfully loaded
	if(!currentMDP->Inuse()) return;

	// set the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	float ratio = (viewingRect.Height() != 0) ? (float)viewingRect.Width() / viewingRect.Height() : (float)viewingRect.Width();
	float fov = 2.0f * atan(13.0f/camera.focalLength) / ratio * 180.0f / M_PI;
	if(ratio > 0.677) fov *= ratio/0.677;
	glLoadIdentity();
	gluPerspective(fov,ratio,0.1f,4096.0f);
	//glOrtho(-viewingRect.Width()/8, viewingRect.Width()/8, -viewingRect.Height()/8, viewingRect.Height()/8, 0.1, 4095);
  
	// Set the viewing transform
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Figure out the location to rotate about on the GL Hominoid
	perihelion::pVector3f hipLocation,worldHip, rotationCenter;
	currentMDP->GetJointDistalByName("Root", hipLocation);
	HominoidToWorldConversion(hipLocation, worldHip);
	rotationCenter[0] = 0;
	rotationCenter[1] = 0;
	rotationCenter[2] = worldHip[2];

	// Move the camera to the correct location in the world. This is done by translating the world NEGATIVE the camera location.
	glTranslated(camera.horizontalOffset/10.0f,-camera.verticalOffset/10.0f,-camera.distance/10.0);

	// Rotate around this point we're looking at by the specified orientations.
	glRotated(camera.altitude, 1.0, 0.0, 0.0);
	glRotated(camera.azimuth, 0.0, -1.0, 0.0);

	// And because the coordinate system is different in openGL than in our world, we rotate the camera to bypass this.
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

	// Now we translate the entire camera to look at some point relative to
	// the (0,0,0) of the rendered hominoid, as specified by the user.
	glTranslatef(-rotationCenter[0], -rotationCenter[1], -rotationCenter[2]);

	// draw axes
	// TODO make call list
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(1,0,0);
	double scale = 0.5;
	double length = 8;
	double tipLength = 2;
	Vector3 origin;
	Skeleton* skel = workingSkeleton();
	origin = pDoc->getOffset(skel, true) - skel->getJoint(JT_ANKLE_CENTER, true);
	pVector3f pOrigin;
	for(int i = 0; i < 3; i++) pOrigin[i] = origin[i];
	SkeletonToWorldConversion(pOrigin);
	glTranslated(pOrigin[0], pOrigin[1], pOrigin[2]);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluCylinder(quadric, scale, scale, length, 10, 10);

	// Sayan 10/11/07 added color to the axis -- red
	// Red is z-axis?
	
	
	glColor3f(1.0,0.0,0.0);

	glTranslated(0, 0, length);
	gluCylinder(quadric, 2*scale, 0, tipLength, 10, 10);
	gluQuadricOrientation(quadric, GLU_INSIDE);
	gluDisk(quadric, 0, 2*scale, 10, 10);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	

	// Sayan 10/11/07 added color to the axis -- green
	// Green is x-axis?

	glColor3f(0.0,1.0,0.0);

	glTranslated(0,0, -length);
	glRotated(90, -1, 0, 0);
	gluCylinder(quadric, scale, scale, length, 10, 10);
	glTranslated(0, 0, length);
	gluCylinder(quadric, 2*scale, 0, tipLength, 10, 10);
	gluQuadricOrientation(quadric, GLU_INSIDE);
	gluDisk(quadric, 0, 2*scale, 10, 10);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);

	// Sayan 10/11/07 added color to the axis -- blue
	// Blue is y-axis?

	glColor3f(0.0,0.0,1.0);
	glTranslated(0,0, -length);
	glRotated(90, 0, 1, 0);
	gluCylinder(quadric, scale, scale, length, 10, 10);
	glTranslated(0, 0, length);
	gluCylinder(quadric, 2*scale, 0, tipLength, 10, 10);
	gluQuadricOrientation(quadric, GLU_INSIDE);
	gluDisk(quadric, 0, 2*scale, 10, 10);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	glTranslated(0,0, -length);
	gluSphere(quadric, scale, 10, 10);
	gluQuadricOrientation(quadric, GLU_INSIDE);

	glPopAttrib();
	glPopMatrix();

	if(modelType != MT_NONE) {
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// Setup some rendering modes
		if(modelType == MT_FLESH) {
		    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glEnable(GL_POLYGON_SMOOTH);  // so edges are smooth, but they are weird now
			//glDisable(GL_DEPTH_TEST);
			//glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_ALPHA);
			//glClear(GL_COLOR_BUFFER_BIT);
		} else if(modelType == MT_WIREFRAME) {
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			//glPolygonMode(GL_BACK, GL_FILL);
			//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		}

		// Convert hominoid from standard 1 mm height to cm, then to correct height
		float height = pDoc->getAnthropometryRef().GetHeight( MetricCns );
		glScalef(0.1f, 0.1f, 0.1f);
		glScalef(height, height, height);

		// Draw the actual hominoid
		const perihelion::pVector3f XAXIS = { 1.0, 0.0, 0.0 };

		perihelion::pVector3f leftFoot, rightFoot, feetCenter;
		currentMDP->GetJointDistalByName("Left Lower Leg", leftFoot);
		currentMDP->GetJointDistalByName("Right Lower Leg", rightFoot);
		perihelion::pVectorAdd(leftFoot, rightFoot, feetCenter);
		pVector3f_SCALE(feetCenter, 0.5, feetCenter);

		//perihelion::pQuat beta;
		//perihelion::mdpQuatFromAngleAxis( -pDoc->getPelvicAngle() * DTOR, XAXIS, beta);
		//perihelion::mdpQuatFromAngleAxis( -gPelvicAngle * DTOR, XAXIS, beta);
		//perihelion::pQuatRotateVector(beta, feetCenter, feetCenter);

		glTranslatef(-feetCenter[0], -feetCenter[1], -feetCenter[2]);
		//glRotated(-pDoc->getPelvicAngle(), 1.0, 0.0, 0.0);
		//glRotated(-pDoc->GetSkeleton().getExtraAngle(EA_PelvisFlexion), 1.0, 0.0, 0.0);
		//glRotated(90, 1.0, 0.0, 0.0);

		bool shoesEnabled = (pDoc->GetFactors().mShoeHeight != 0);
		if(currentMDP->GetMesh(2)) currentMDP->GetMesh(2)->Enable(shoesEnabled);
		if(currentMDP->GetMesh(3)) currentMDP->GetMesh(3)->Enable(shoesEnabled);
		bool male = !(bool)(pDoc->GenderCode());
		currentMDP->GetMesh(1)->UpdateVertexColors((float)skinColor[0]/255,(float)skinColor[1]/255,(float)skinColor[2]/255);
		if(male) {
			shirtColor = maleShirtColor;
			pantsColor = malePantsColor;
		} else {
			shirtColor = femaleShirtColor;
			pantsColor = femalePantsColor;
		}
		currentMDP->UpdateVertexShirtColors((float)shirtColor[0]/255,(float)shirtColor[1]/255,(float)shirtColor[2]/255,male, MaleBoneListShirt, FemaleBoneListShirt);
		currentMDP->UpdateVertexPantColors((float)pantsColor[0]/255,(float)pantsColor[1]/255,(float)pantsColor[2]/255,male, MaleBoneListPant, FemaleBoneListPant);

		currentMDP->Draw(false, true);

		glPopMatrix();
		glPopAttrib();
	}

	// Sayan 08/30/07
	// We want to display the red dot representing the target 
	// spssid, in the Oblique view as well. (It is already being
	// represented in the top view, front view, etc.)




	if(drawTarget) {
		glPushMatrix();
		// have to go back to the origin we computed before
		glTranslated(pOrigin[0], pOrigin[1], pOrigin[2]);

		// debug: draw a sphere at the origin
		// glColor3ubv(GLSkeleton::skeletonColor);
		// gluSphere(quadric, 4, 10, 10);

		// This draws the target (red dot) for the "Reach to grasp" experiment
		//Vector3 drawTarg = targetLocation;// + pDoc->getOffset(skel, true) - skel->getJoint(JT_ANKLE_CENTER, true);
		Vector3 drawTarg;
		if(pDoc->getPosition() == P_Standing) {
			drawTarg = targetLocation + skel->getJoint(JT_LBALL, true) - pDoc->getOffset(skel, true);
		} else {
			drawTarg = targetLocation + skel->getJoint(JT_HIP_CENTER, true) - pDoc->getOffset(skel, true);
		}
		//glTranslated(targetLocation[0], targetLocation[1], targetLocation[2]);
		glTranslated(drawTarg[0], drawTarg[1], drawTarg[2]);
		//Vector3 hand = skel->getJoint(JT_RHAND, true) - skel->getJoint(JT_ANKLE_CENTER, true);
		//glTranslated(hand[0], hand[1], hand[2]);
		
		glColor3ubv(GLSkeleton::forceColor);
		gluSphere(quadric, 4, 10, 10);
		glColor3ubv(GLSkeleton::backgroundColor);
		gluSphere(quadric, 2.5, 10, 10);
		glColor3ubv(GLSkeleton::forceColor); ///Yijun??
		gluSphere(quadric, 1, 10, 10);
		glColor3ubv(GLSkeleton::rightfeetforceColor);
		gluSphere(quadric, 4, 10, 10);
		glColor3ubv(GLSkeleton::rightfeetforceColor); ///Yijun??
		gluSphere(quadric, 1, 10, 10);
		glColor3ubv(GLSkeleton::leftfeetforceColor);
		gluSphere(quadric, 4, 10, 10);
		glColor3ubv(GLSkeleton::leftfeetforceColor); ///Yijun??
		gluSphere(quadric, 1, 10, 10);
		glPopMatrix();
	}
		if(showFrameText){
				//drawtext(pDoc->getCurrentFrame());
				if ((getFrame() >= 0) && (getFrame()<1000))
				{
										drawtext(getFrame());
				}else
				{
					drawtext(pDoc->getCurrentFrame());
				}
		}

	// LEAVE THE FOLLOWING CODE IN FOR DEBUGGING PURPOSES
	if(0) {
		glPushMatrix();
		glColor3f(1.0,0,0);
		glPointSize(10);
		int frame = pDoc->getCurrentFrame();
		glBegin(GL_POINTS);
		if(frame < 100)
		for(int i = 0; i < NUM_JOINTS; i++) {
			glVertex3dv(&motionvecs[frame][i][0]);
		}
		glEnd();
		glPopMatrix();
	}

	if(0) {
		// Draw red dots at all of the joint locations.  Note, needs to be set to metric, and
		// anchor points in two coordinate systems not equivalent.  The -7 on z is to correct
		// for the ankle vs. heel height difference, as our anchor point is at the center of
		// the ankles.  It rather should be at the left heel.
		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(8);
		Length   lLocX( 0, 7 );
		Length   lLocY( 0, 7 );
		Length   lLocZ( 0, 7 );

	    #define CVT { loc[0] = lLocX.Value(); loc[1] = lLocY.Value(); loc[2] = lLocZ.Value(); SkeletonToWorldConversion(loc); glVertex3f(loc[0], loc[1], loc[2]); }
		Vector3 originOffset;
		originOffset = ((skel->getJoint(JT_LANKLE, true) + skel->getJoint(JT_RANKLE, true)) / 2);
						//-lSkeleton.getJoint(JT_ANKLE_CENTER);
		Vector3 temp;
		pVector3f temp2;
		#define OMG(X) temp = skel->getJoint(X) - originOffset; glVertex3dv(&temp[0]);
		#define OMG3(X) temp = X; temp -= skel->getJoint(JT_ANKLE_CENTER, true); for(int i = 0; i < 3; i++) temp2[i] = temp[i]; SkeletonToWorldConversion(temp2); glVertex3fv(temp2);
		#define OMG2(X) OMG3(skel->getJoint(X, true))
		glBegin(GL_POINTS);
		for(int i = 0; i < NUM_JOINTS; i++) {
			OMG2((JointID)i);
		}
		glVertex3dv(&Vector3(0,0,0)[0]);
		// OMG3(skel->getSeatReference(true))
		// OMG3(skel->getLeftSeatFront(true))
		// OMG3(skel->getRightSeatFront(true))
		// OMG3(skel->getSeatCenter(true))
		// OMG3(skel->getSeatBackCenter(true))
		// glVertex3dv(((lSkeleton.getJoint(JT_LANKLE) + lSkeleton.getJoint(JT_RANKLE)) / 2)._els);
		// glVertex3dv(lSkeleton.getJoint(JT_ANKLE_CENTER)._els);

		glEnd();
		glPopMatrix();
	}
	if(0) {
		// DEBUGGING
		// draw blue dots at locations computed from posture angles
				glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(10);
		const int num_joints = 21;
		JointID jointArray[] = {JT_LWRIST, JT_RWRIST, JT_LSHOULDER, JT_RSHOULDER,
			JT_LELBOW, JT_RELBOW, JT_LHAND, JT_RHAND,
			JT_LBALL, JT_RBALL, JT_LANKLE, JT_RANKLE,
			JT_LKNEE, JT_RKNEE, JT_LHIP, JT_RHIP,
			JT_SCJ, JT_L5S1_TEMP, JT_C7T1, JT_TRAGION, JT_NASION};

		Vector3 joints[NUM_JOINTS];
		Vector3 rback,lback;
		skel->SetPostureFromPA(joints,rback,lback);
		Vector3 new_joints[num_joints];
		Vector3 skel_joints[num_joints];
		for(int i = 0; i < num_joints; i++) {
			new_joints[i] = joints[jointArray[i]];
			skel_joints[i] = skel->getJoint((JointID)i, true);
		}
		Vector3 ankle_center = (joints[JT_RANKLE] + joints[JT_LANKLE]) / 2;
		Vector3 temp;
		pVector3f temp2;
		#define DRW3(X) temp = X; temp -= ankle_center; for(int i = 0; i < 3; i++) temp2[i] = temp[i]; SkeletonToWorldConversion(temp2); glVertex3fv(temp2);
		#define DRW2(X) OMG3(joints[X])

		for(int i = 0; i < num_joints; i++) {
			glColor3f(0,i*1.0/num_joints,1-i*1.0/num_joints);
			glBegin(GL_POINTS);
			DRW2(jointArray[i]);
			glEnd();
		}
		glBegin(GL_POINTS);
		DRW3(rback);
		DRW3(lback);
		glEnd();
		
		glPopMatrix();
	}

	RenderEnvironment();

	// Draw the background again, for transparency purposes
	if(bgActive) {
		// this will se lighting, blend, etc on its own
		drawBackground(bgAlpha);
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

void GLOblique::setframenum(bool show) {
	showFrameText = show;
}
bool GLOblique::getframenum(void) {
	return showFrameText;
}

void GLOblique::drawtext(int frame)
{
	//for frame number shows

GLuint base;//the first display list we create

glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background
glClearDepth(1.0f);                                 // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations

HFONT   font;                                   // Used For Good House Keeping
base = glGenLists(96); 
font = CreateFont(  
    12,                            // Height Of Font
    6,                              // Width Of Font
    0,                              // Angle Of Escapement
    0,                              // Orientation Angle
    FW_BOLD,                        // Font Weight
    FALSE,                          // Italic
    FALSE,                          // Underline
    FALSE,                          // Strikeout
    ANSI_CHARSET,                   // Character Set Identifier
    OUT_TT_PRECIS,                  // Output Precision
    CLIP_DEFAULT_PRECIS,            // Clipping Precision
    ANTIALIASED_QUALITY,            // Output Quality
    FF_DONTCARE|DEFAULT_PITCH,      // Family And Pitch
    "Times New Roman"); 
HGLRC hRC;
// create a rendering context
hRC = wglCreateContext(gl_hDC);
// make it the calling thread's current rendering context  
wglMakeCurrent (gl_hDC, hRC); 
// now we can call OpenGL API  

// make the system font the device context's selected font  
SelectObject (gl_hDC, font); 

wglUseFontBitmaps(gl_hDC, 32, 96, base);               // Builds 96 Characters Starting At Character 32
//SelectObject(gl_hDC, oldfont);                         // Selects The Font We Want
DeleteObject(font); 
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
glLoadIdentity();                                   // Reset The Current Modelview Matrix
glTranslatef(0.0f,0.0f,-1.0f);                      // Move One Unit Into The Screen
// Pulsing Colors Based On Text Position
glColor3f(0.0f,0.0f,0.0f);
// Position The Text On The Screen
glRasterPos2f(0.8, -0.8);
//glRasterPos2f(0,0);
//SwapBuffers(gl_hDC);// Everything Went OK
//char* mt = "Frame:";  // Print GL Text To The Screen
char        text[256];                              // Holds Our String
va_list     ap;                                     // Pointer To List Of Arguments


itoa(frame,text,10);
String fmts = "Frame: ";
fmts += text;
char* fmt = fmts;
va_start(ap, fmt);                                  // Parses The String For Variables
vsprintf(text, fmt, ap);                        // And Converts Symbols To Actual Numbers
va_end(ap);                                         // Results Are Stored In Text

glPushAttrib(GL_LIST_BIT);                          // Pushes The Display List Bits
glListBase(base - 32);                              // Sets The Base Character to 32
glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
glPopAttrib(); 

//glViewport(0,0,(GLsizei)w,(GLsizei)h);
//glMatrixMode(GL_PROJECTION);
//glLoadIdentity();
//gluPerspective(60.0f,(GLfloat)w / (GLfloat)h,1,100);
//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
//glutKeyboardFunc(keyboard);
//glutMainLoop();
//	wglMakeCurrent(0,0);
wglMakeCurrent (NULL, hRC);
wglDeleteContext(hRC);
wglMakeCurrent (gl_hDC, gl_hRC); 
glDeleteLists(base,96);
return;
}

void GLOblique::drawBackground(float alpha) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,bgTexture);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f,1.0f,1.0f,alpha);

	float right = 1.0f,top = 1.0f;

	if(bgMode == PM_CENTER) {
		//CRect client;
		//GetClientRect(&client);

		long bgWidth,bgHeight;
		bgPicture->get_Width(&bgWidth);
		bgPicture->get_Height(&bgHeight);

		// this would keep the image the size it actually is, if we can find that out
		
		// CDC* cdc = GetDC();
		// HDC hdc;
		// 
		// if(cdc && (hdc = cdc->m_hDC)) {
		// 	float pixelWidth = bgWidth * GetDeviceCaps(hdc, LOGPIXELSX) / 2540;
		// 	float pixelHeight = bgHeight * GetDeviceCaps(hdc, LOGPIXELSY) / 2540;
		// 	right = pixelWidth / client.Width();
		// 	top = pixelHeight / client.Height();
		// }
		// 
		// this fits closest dimension to the window, and preserve image ratio
//		else
//		{
		if(bgWidth*viewingRect.Height() > viewingRect.Width() * bgHeight)//client.Height() > client.Width()*bgHeight)
	            top *= (float)viewingRect.Width()*bgHeight / (bgWidth*viewingRect.Height());
			else
	            right /= (float)viewingRect.Width()*bgHeight / (bgWidth*viewingRect.Height());
//		}

	}

	glBegin(GL_QUADS);
		glTexCoord2i(0,0);
		glVertex3f(-right,-top,0);
		glTexCoord2i(1,0);
		glVertex3f(right,-top,0);
		glTexCoord2i(1,1);
		glVertex3f(right,top,0);
		glTexCoord2i(0,1);
		glVertex3f(-right,top,0);
	glEnd();

	glPopAttrib();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
}
void GLOblique::updateBackground(void) {
	HDC			hdcTemp;										// Temp DC to hold our bitmap
	HBITMAP		hbmpTemp;										// Temp bitmap
	long		lWidth;											// Width(logical units)
	long        lWidthPixels;                                   // Width (pixels, rounded to a power of 2)
	long		lHeight;										// Height(logical units)
	long        lHeightPixels;                                  // Height (pixels, rounded to a power of 2)
	GLint		glMaxTexDim;									// Maximum texture size

	// Create a temporary device context to render bitmap to
	hdcTemp = CreateCompatibleDC(::GetDC(0));
	if(!hdcTemp) {
		AfxMessageBox("Could not create temporary device context to render bitmap");
		return;
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
	bgPicture->get_Width(&lWidth);
	lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	bgPicture->get_Height(&lHeight);
	lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize image to closest power of two to ensure compatibility
	if(lWidthPixels <= glMaxTexDim)
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f); 
	else
		lWidthPixels = glMaxTexDim;

	if(lHeightPixels <= glMaxTexDim)
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	else
		lHeightPixels = glMaxTexDim;

	//	Create A Temporary Bitmap
	BITMAPINFO bi = {0};
	DWORD* pBits;

	// Fill the bitmap structure
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set structure size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= lWidthPixels;					        // Width that is a power of 2
	bi.bmiHeader.biHeight		= lHeightPixels;					    // Height that is a power of 2
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	// Create bitmap
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS,(void**)&(pBits), 0, 0);
	if(hbmpTemp == NULL) {												// Ensure that it succeeded
		DeleteDC(hdcTemp);
		AfxMessageBox("Could not load create bitmap");
		return;
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render IPicture to the bitmap
	bgPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// Convert from BGR to RGB Format and set alpha to 255
    for(long i = 0; i < lWidthPixels * lHeightPixels; ++i) {
		BYTE* pPixel = (BYTE*)(&(pBits[i]));
		BYTE  temp = pPixel[0];
		pPixel[0] = pPixel[2];
		pPixel[2] = temp;
		pPixel[3] = 255;
	}

	glGenTextures(1, &bgTexture);
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)pBits);
}
void GLOblique::RenderEnvironment() {

	if(!currentMDP->Inuse())
		return;  // Don't render if hominoid is not loaded

	glColor3ubv(handHeldColor);

	perihelion::pVector3f leftAnkle, rightAnkle,
		leftUpperLeg, rightUpperLeg,
		rightHand, leftHand,
		midFront, midNormal, midPoint;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Make sure cull face is disabled so that the objects render correctly
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// This renders the chair for a supported calculation.  
	//if ((pDoc->getPositioning() & 1))
	if(pDoc->getPosition() == P_Sitting) {
		this->RenderChair(leftAnkle, rightAnkle, midNormal, midFront);
		//if (pDoc->getPositioning() & HAS_BACK)	// mSeatBack 1 (true) when the back is supported
		if(pDoc->hasSeatBackRest())
			this->RenderChairBack();
	}
	if (floorEnabled) {
		currentMDP->GetJointDistalByName("Left Ankle", leftAnkle);
		currentMDP->GetJointDistalByName("Right Ankle", rightAnkle);

		HominoidToWorldConversion(leftAnkle, leftAnkle);
		HominoidToWorldConversion(rightAnkle, rightAnkle);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);

		if(!AreFeetLevel()) {
			FeetSupport_e feetSupport = pDoc->getFeetSupport();
			//if(pDoc->getStance() != STANCE_RIGHT_FOOT && pDoc->getStance() != STANCE_NO_FEET)
			if(feetSupport != FS_Right && feetSupport != FS_Neither) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				RenderFloor(leftAnkle,  OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH);
				if(handheld.outline) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glColor3ubv(handHeldOutlineColor);
					RenderFloor(leftAnkle, OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH);
				}
			}
			//if(pDoc->getStance() != STANCE_LEFT_FOOT && pDoc->getStance() != STANCE_NO_FEET)
			if(feetSupport != FS_Left && feetSupport != FS_Neither) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3ubv(handHeldColor);
				RenderFloor(rightAnkle, OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH);
				if(handheld.outline) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glColor3ubv(handHeldOutlineColor);
					RenderFloor(rightAnkle, OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH);
				}
			}
		} else {
			currentMDP->GetJointProximalByName("Left Upper Leg", leftUpperLeg);
			currentMDP->GetJointProximalByName("Right Upper Leg", rightUpperLeg);
			HominoidToWorldConversion(leftUpperLeg, leftUpperLeg);
			HominoidToWorldConversion(rightUpperLeg, rightUpperLeg);

			// Find the midPoint of the two midpoints
			midPoint[0] = (leftUpperLeg[0] + rightUpperLeg[0])/2.0f;
			midPoint[1] = (((leftUpperLeg[1] + rightUpperLeg[1])/2.0f)
							+ ((leftAnkle[1] + rightAnkle[1])/2.0f)/2.0f);
			midPoint[2] = (leftAnkle[2] + rightAnkle[2])/2.0f;

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3ubv(handHeldColor);
			RenderFloor(midPoint, COMBINED_OBL_FLOOR_WIDTH, COMBINED_OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH);
			if(handheld.outline) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3ubv(handHeldOutlineColor);
				RenderFloor(midPoint, COMBINED_OBL_FLOOR_WIDTH, COMBINED_OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH);
			}
		}
	}
	if (handheld.type != HHO_NONE) {

		Skeleton* skel = workingSkeleton();
		Length lLocX(0, 2);
		Length lLocY(0, 2);
		Length lLocZ(0, 2);

		Vector3 origin = skel->getJoint(JT_ANKLE_CENTER, true);
		Vector3 temp = skel->getJoint(JT_LHAND, true) - origin;
		//lSkeleton.C4EPoint(JT_LHAND, lLocX, lLocY, lLocZ);
		leftHand[0] = temp[0];//lLocX.ValueIn(MetricCns); //+ avgPoint[0];
		leftHand[1] = temp[1];//lLocY.ValueIn(MetricCns); //+ avgPoint[1];
		leftHand[2] = temp[2];//lLocZ.ValueIn(MetricCns);
		SkeletonToWorldConversion(leftHand);
		temp = skel->getJoint(JT_RHAND, true) - origin;
		//lSkeleton.C4EPoint(JT_RHAND, lLocX, lLocY, lLocZ);
		rightHand[0] = temp[0];//lLocX.ValueIn(MetricCns); //+ avgPoint[0];
		rightHand[1] = temp[1];//lLocY.ValueIn(MetricCns); //+ avgPoint[1];
		rightHand[2] = temp[2];//lLocZ.ValueIn(MetricCns);
		SkeletonToWorldConversion(rightHand);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		RenderHandHeldObjects(leftHand, rightHand);
	}
	glPopAttrib();
	if(pDoc->getBarType() != BT_NOBARRIER) {
		RenderBarrier();
	}
}
void GLOblique::RenderHandHeldObjects(perihelion::pVector3f& leftHand, perihelion::pVector3f& rightHand) {
	double magnitudeX, magnitudeY, magnitudeZ, magnitudeXY, magnitudeXYZ, theta, phi;
	perihelion::pVector3f midPoint;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Find the midpoint
	midPoint[0] = (rightHand[0] + leftHand[0]) / 2.0f;
	midPoint[1] = (rightHand[1] + leftHand[1]) / 2.0f;
	midPoint[2] = (rightHand[2] + leftHand[2]) / 2.0f;

	glColor3ubv(handHeldColor);

	glPushMatrix();
	if(handheld.type != HHO_CYLINDER)
		glTranslatef(midPoint[0], midPoint[1], midPoint[2]);
	else if (handheld.type == HHO_CYLINDER)
		glTranslatef(rightHand[0] - LOC_CORRECTION, rightHand[1], rightHand[2]);

	// Find the distances between in the hands in each direction
	magnitudeX = rightHand[0] - leftHand[0];
	if(handheld.type != HHO_SPHERE)
		magnitudeX = magnitudeX - MAG_CORRECTION;
	else
		magnitudeX = magnitudeX - SPHERE_MAG_CORRECT;

	magnitudeY = rightHand[1] - leftHand[1];
	magnitudeZ = rightHand[2] - leftHand[2];
	magnitudeXY = sqrt(pow(magnitudeX, 2) + pow(magnitudeY, 2));
	magnitudeXYZ = sqrt(pow(magnitudeX, 2) + pow(magnitudeY, 2) + pow(magnitudeZ, 2));

	// Calculate the horizontal angle
	theta = atan(magnitudeY/magnitudeX);
	phi = atan(magnitudeZ/magnitudeXY);
	// Rotate the object
	if(handheld.type != HHO_CYLINDER) {
		glRotated(theta * 180.0f/M_PI, 0.0, 0.0, 1.0);
		glRotated(phi* 180.0f/M_PI, sin(theta), -cos(theta),0.0);
	} else {
		// The rotation is different for the cylinder because
		// it needs to be rotated about the y-axis first
		glRotated(-90, 0.0, 1.0, 0.0);
		glRotated(theta * 180.0f/M_PI, 1.0, 0.0, 0.0);
		glRotated(phi * 180.0f/M_PI, 0.0, -cos(theta), -sin(theta));
	}
	if (handheld.type != HHO_CYLINDER)
		glScalef(float(magnitudeXYZ),handheld.depth / 10.0,handheld.height / 10.0);
	else if(handheld.type == HHO_CYLINDER) {
		// An extra rotation is needed for the cylinder otherwise it would be
		//    standing straight up from the hand
		glScaled(handheld.height / 10.0,handheld.depth / 10.0,magnitudeXYZ);
	}

	glScalef(.5, .5, .5);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0,1.0);

	if(handheld.type == HHO_SPHERE) {
		gluSphere(quadric,1.0, 20, 20);
		if(handheld.outline) {
			glColor3ubv(handHeldOutlineColor);
			gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
			gluSphere(quadric, 1.0f, 20, 20);
			gluQuadricDrawStyle(quadric, GLU_FILL);
		}
    } else if(handheld.type == HHO_CYLINDER) {
		gluCylinder(quadric, 1.0, 1.0, 2.0, 20, 20);
		gluDisk(quadric, 0.0, 1.0, 20, 20);
		glTranslatef(0.0, 0.0, 2.0);
		gluDisk(quadric, 0.0, 1.0, 20, 20);
		if(handheld.outline) {	
			glColor3ubv(handHeldOutlineColor);
			glTranslatef(0.0, 0.0, -2.0f);
			gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
			gluCylinder(quadric, 1.0, 1.0, 2.0, 20, 20);
			gluDisk(quadric, 0.0, 1.0, 20, 20);
			glTranslatef(0.0, 0.0, 2.0);
			gluDisk(quadric, 0.0, 1.0, 20, 20);
			gluQuadricDrawStyle(quadric, GLU_FILL);
		}
	} else if (handheld.type == HHO_BOX) {
		RenderCube();
		if(handheld.outline) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3ubv(handHeldOutlineColor);
			RenderCube();
		}
	}

	glPopAttrib();
	glPopMatrix();
}
void GLOblique::RenderBarrier() {
	perihelion::pVector3f leftFoot, rightFoot, hipLocation, lowestFoot;
	// Pop the matrix so the barriers render in the correct location
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0,1.0);

	glDisable(GL_CULL_FACE);

	if (pDoc->getBarType() != BT_NOBARRIER) {
		currentMDP->GetJointDistalByName("Left Ankle", leftFoot);
		currentMDP->GetJointDistalByName("Right Ankle", rightFoot);
		currentMDP->GetJointDistalByName("Root", hipLocation);

		HominoidToWorldConversion(leftFoot, leftFoot);
		HominoidToWorldConversion(rightFoot, rightFoot);
		HominoidToWorldConversion(hipLocation, hipLocation);

		if (leftFoot[2] < rightFoot[2])
				for (int i = 0; i < 3; i++){
 				lowestFoot[i] = leftFoot[i];}
		else
				for (int i = 0; i < 3; i++){
 				lowestFoot[i] = rightFoot[i];}

		// Make it so the floor is measured from the lowest foot
glTranslatef((leftFoot[0]+rightFoot[0])/2, lowestFoot[1], lowestFoot[2]);
		float height = pDoc->getAnthropometryRef().GetHeight( MetricCns );

		// Convert from mm to cm
		glScaled(0.1, 0.1, 0.1);


		glRotatef(float(pDoc->getBarAzimuth()), 0.0, 0.0, 1.0); 
		glTranslatef(0.0, float(pDoc->getBarDistance()),
							float(pDoc->getBarElevation()));
		// if the barrier type is BT_WALL there are no rotations 
		if (pDoc->getBarType() == BT_WALL) {
			// JTK - This odd rotation and translation was done to make the shading on the 
			// wall look correct.  This is really just a a glRotatef(90.0f, 1.0f, 0.0f, 0.0f)
			// but for some reason that did not appear correctly.
			glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0, -float(pDoc->getBarWidth()), 0.0 );
		} else {
			glTranslatef(0.0, 0.0, -pDoc->getBarThickness());
		}
		glTranslatef(0.0, float(pDoc->getBarWidth()), 0.0 );
 		glRotatef(float(pDoc->getBarPitch())-90, 1.0, 0.0, 0.0); 
		glTranslatef(0.0, -float(pDoc->getBarWidth()), 0.0 );
		glScalef(float(pDoc->getBarLength()), float(pDoc->getBarWidth()),
												float(pDoc->getBarThickness()));

		glScalef(0.5, 0.5, 0.5);
		RenderBarrierCube();

		if(handheld.outline) {
			glColor3ubv(handHeldOutlineColor);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			RenderBarrierCube();
		}
	}
	glPopMatrix();
	glPopAttrib();
}
void GLOblique::RenderChairBack() {
	perihelion::pVector3f midPoint;
	Skeleton* skel = workingSkeleton();
	double flexion = skel->GetAngle(A_TrunkFlexion);
	double rotation = skel->GetAngle(A_TrunkAxialRotation);
	//Length lLocX(0,7), lLocY(0,7), lLocZ(0,7);
	//lSkeleton.C4EPoint(JT_SEATBACK_CENTER, lLocX, lLocY, lLocZ);
	Vector3 seatBack = skel->getSeatBackCenter(true) - skel->getJoint(JT_ANKLE_CENTER, true);
	midPoint[0] = seatBack[0];//lLocX.ValueIn(MetricCns);
	midPoint[1] = seatBack[1];//lLocY.ValueIn(MetricCns);
	midPoint[2] = seatBack[2];//lLocZ.ValueIn(MetricCns);

	glPushMatrix();
	double angle = flexion - 90.0f;//currentRef[A_TrunkFlexion] - 90.0f;
	SkeletonToWorldConversion(midPoint);
	glTranslatef(midPoint[0], midPoint[1], midPoint[2]); 

	//if (currentRef[A_TrunkFlexion] > 90) // don't rotate forward of vertical
	glRotatef(angle, 1.0f, 0.0f, 0.0f);

	glTranslatef(0, 0, -CHAIR_BACK_HEIGHT);
   
	//glRotatef(currentRef[A_TrunkAxialRotation], 0.0f, 0.0f, 1.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	double temp = (pDoc->getBackRestHeight().ValueIn(MetricCns));
	glScalef(CHAIR_SEAT_WIDTH, CHAIR_BACK_DEPTH, CHAIR_BACK_HEIGHT);	// The hips are scaled by .5 and scaled by another .5 because the cube is 2x2x2 so .25 
	glDisable(GL_CULL_FACE);
	RenderCube();
	glPopMatrix();
}
void GLOblique::RenderChair(perihelion::pVector3f& leftAnkle, perihelion::pVector3f& rightAnkle, perihelion::pVector3f& midNormal, perihelion::pVector3f& midFront) {
	//perihelion::pVector3f midPoint;
	Skeleton* skel = workingSkeleton();
	JointAngles currentRef = skel->GetAngles();
	//lSkeleton.GetAngles(lSkeleton.getCurrentFrame(),currentRef);

	Vector3 temp;
	Vector3 ankleCenter = skel->getJoint(JT_ANKLE_CENTER, true);
	Length lLocX(0,7), lLocY(0,7), lLocZ(0,7);
	perihelion::pVector3f seatRef, forwardCTR, leftFront, rightFront;
	//lSkeleton.C4EPoint(JT_SEAT_REF, lLocX, lLocY, lLocZ);
	temp = skel->getSeatReference(true) - ankleCenter;
	seatRef[0] = temp[0];//lLocX.ValueIn(MetricCns);
	seatRef[1] = temp[1];//lLocY.ValueIn(MetricCns);
	seatRef[2] = temp[2];//lLocZ.ValueIn(MetricCns);
   
	//lSkeleton.C4EPoint(JT_SEAT_CENTER, lLocX, lLocY, lLocZ);
	temp = skel->getSeatCenter(true) - ankleCenter;
	forwardCTR[0] = temp[0];//lLocX.ValueIn(MetricCns);
	forwardCTR[1] = temp[1];//lLocY.ValueIn(MetricCns);
	forwardCTR[2] = temp[2];//lLocZ.ValueIn(MetricCns);
   
	//lSkeleton.C4EPoint(JT_LSEAT_FRONT, lLocX, lLocY, lLocZ);
	temp = skel->getLeftSeatFront(true) - ankleCenter;
	leftFront[0] = temp[0];//lLocX.ValueIn(MetricCns);
	leftFront[1] = temp[1];//lLocY.ValueIn(MetricCns);
	leftFront[2] = temp[2];//lLocZ.ValueIn(MetricCns);
   
	//lSkeleton.C4EPoint(JT_RSEAT_FRONT, lLocX, lLocY, lLocZ);
	temp = skel->getRightSeatFront(true) - ankleCenter;
	rightFront[0] = temp[0];//lLocX.ValueIn(MetricCns);
	rightFront[1] = temp[1];//lLocY.ValueIn(MetricCns);
	rightFront[2] = temp[2];//lLocZ.ValueIn(MetricCns);
	SkeletonToWorldConversion(seatRef);
	SkeletonToWorldConversion(forwardCTR);
	SkeletonToWorldConversion(leftFront);
	SkeletonToWorldConversion(rightFront);

	double width = fabs(rightFront[0] - leftFront[0]);
	double lengthY = forwardCTR[1] - seatRef[1];
	double lengthZ = forwardCTR[2] - seatRef[2];
	double magnitude = sqrt(pow(lengthY,2) + pow(lengthZ, 2));
	double slope = 0;
	if(lengthY > 0.001 || lengthZ < 0.001) slope = (lengthY / lengthZ);
	double height = fabs(rightFront[2] - seatRef[2]);
	double centerX = seatRef[0];
	double centerY = 0;
	double centerZ = 0;// + (lengthZ / 2.0);
	double yAverage = (seatRef[1] + forwardCTR[1]) / 2.0;
	double zAverage = (seatRef[2] + forwardCTR[2]) / 2.0;
	double firstTerm = 0;
	if(!((forwardCTR[2] - seatRef[2]) < 0.001 && (forwardCTR[2] - seatRef[2]) > -0.001)) {
		firstTerm = (seatRef[1] - forwardCTR[1]) / (forwardCTR[2] - seatRef[2]);
		double sign = 1;
		if((forwardCTR[1] - seatRef[1]) / (forwardCTR[2] - seatRef[2]) < 0) {
			sign = -1;
		}
		// Generate the two solutions then check to see which one is correct
		double xOne, yOne;
		xOne = ((sign * CHAIR_SEAT_HEIGHT) / (sqrt(1+(firstTerm*firstTerm)))) + yAverage;
		yOne = firstTerm * (sign * CHAIR_SEAT_HEIGHT / (sqrt(1 + firstTerm*firstTerm))) + zAverage;
		centerY = xOne;
		centerZ = yOne;
	} else {
		centerY = yAverage;
		centerZ = zAverage - CHAIR_SEAT_HEIGHT;
	}
	float yOffset = 0;			// Don't add anything if the front is not supported
	glDisable(GL_CULL_FACE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	if (quadric == NULL)
		quadric = gluNewQuadric();

	double upperLegAngle = 0;
	// Render the seat
	glTranslatef(centerX, centerY, centerZ);
	// Rotate the seat pan so that it is rotated the average of the upper leg angles
	// i dont think this averaging happens anymore, this just takes the angle of
	// whichever leg has the lower knee.
	float upperLegAngleAvg = 0;
	skel->C4EPoint(JT_LKNEE, lLocX, lLocY, lLocZ);
	double leftKnee = lLocZ.ValueIn(MetricCns);
	skel->C4EPoint(JT_RKNEE, lLocX, lLocY, lLocZ);
	double rightKnee = lLocZ.ValueIn(MetricCns);
	if(leftKnee < rightKnee) {
		upperLegAngle = currentRef[A_UpperlegLV];
	} else {
		upperLegAngle = currentRef[A_UpperlegRV];
	}
	if(upperLegAngle < 0) upperLegAngle += 180;
	else					 upperLegAngle -= 180;
	upperLegAngle -= 180;
   
	glRotatef(upperLegAngle, 1.0f, 0.0f, 0.0f);
	glScalef(width, (magnitude)/2, CHAIR_SEAT_HEIGHT);

	gluCylinder(quadric, 1.0, 1.0, 1.0, 32, 32);
	gluDisk(quadric, 0.0, 1.0, 32, 32);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluDisk(quadric, 0.0, 1.0, 32, 32);

	glPopMatrix();
	glPopAttrib();
}
void GLOblique::RenderChairSupport(float x, float y, float z, float seatLocation, bool chairBack) {
	glPushMatrix();
	//glTranslated(leftAnkle[0] - .25, midPoint[1] - 0.45, leftAnkle[2]);
	glTranslated(x, y, z);
	//JointAngles currentRef;
	//unsigned int frame = GetDocument()->GetSkeleton().getCurrentFrame();
	//GetDocument()->GetSkeleton().GetAngles(frame,currentRef);
	Skeleton* skel = workingSkeleton();
	double flexion = skel->GetAngle(A_TrunkFlexion);
	if (flexion > 90 && chairBack ) {
		glRotatef(flexion - 90.0f, 1.0f, 0.0f, 0.0f);
	}
	glScalef(CHAIR_LEG_WIDTH, CHAIR_LEG_DEPTH, seatLocation);
	gluCylinder(quadric, 1.0, 1.0, 1.0, 32, 32);
	gluDisk(quadric, 0.0, 1.0, 32, 32);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluDisk(quadric, 0.0, 1.0, 32, 32);
	glPopMatrix();
}
void GLOblique::RenderCube(void) {
	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0,-1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, 1.0);

		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-1.0, 1.0,  1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0,-1.0);
		glVertex3f(-1.0, -1.0, 1.0);

		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);

		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, 1.0,-1.0);
		glVertex3f(1.0, 1.0, -1.0);

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);

		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0,-1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0); 
	glEnd();
}
void GLOblique::RenderBarrierCube(void) {
	// This function is very similar to RenderCube except that the z value is from 
	// [0, 2] so that the measurement of elevation works out correctly.
	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 2.0, 2.0);
		glVertex3f(1.0, 2.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 2.0);

		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-1.0, 2.0,  2.0);
		glVertex3f(-1.0, 2.0, 0.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, 2.0);

		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, 0.0, 2.0);
		glVertex3f(-1.0, 0.0, 2.0);
		glVertex3f(-1.0, 2.0, 2.0);
		glVertex3f(1.0, 2.0, 2.0);

		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glVertex3f(-1.0, 2.0, 0.0);
		glVertex3f(1.0, 2.0, 0.0);

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(1.0, 2.0, 0.0);
		glVertex3f(-1.0, 2.0, 0.0);
		glVertex3f(-1.0, 2.0, 2.0);
		glVertex3f(1.0, 2.0, 2.0);

		glNormal3f(0.0, -1.0, 0.0);			
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, 2.0);
		glVertex3f(1.0, 0.0, 2.0); 
	glEnd();
}
void GLOblique::RenderFloor(perihelion::pVector3f& heel, float floorWidth, float floorHeight, float floorDepth) {

	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2]); 
		glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2]);
		glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2]);
		glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2]);
	
		// Sides
		glNormal3f(1,0,0);
		glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2]); 
		glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2]);
		glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2] - floorDepth); 
		glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);

		glNormal3f(0,1,0);
		glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2]);
		glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2]);
		glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);
		glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);

		glNormal3f(-1,0,0);
		glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2]);
		glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2]);
		glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);
		glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);

		glNormal3f(0,-1,0);
		glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2]);
		glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2]); 
		glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2] - floorDepth); 
		glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);

		// Bottom
		glNormal3f(0,0,-1);
		glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2] - floorDepth); 
		glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);
		glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);
		glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);
	glEnd();
}

bool GLOblique::init() {
	gluQuadricDrawStyle(quadric,GLU_FILL);
	gluQuadricOrientation(quadric,GLU_INSIDE); 

	return true;
}

void GLOblique::HominoidToWorldConversion(const perihelion::pVector3f &in, perihelion::pVector3f &out) {

	const perihelion::pVector3f XAXIS = { 1.0, 0.0, 0.0 };
	float height = pDoc->getAnthropometryRef().GetHeight( MetricCns );
	//float pelvicAngle = pDoc->getPelvicAngle();

	perihelion::pVector3f leftFoot, rightFoot, root, intermediate;
	currentMDP->GetJointDistalByName("Left Lower Leg", leftFoot);
	currentMDP->GetJointDistalByName("Right Lower Leg", rightFoot);
	perihelion::pVectorAdd(leftFoot, rightFoot, root);
	pVector3f_SCALE(root, 0.5, root);
	pVector3f_COPY(in, intermediate);

	//perihelion::pQuat beta;
	//perihelion::mdpQuatFromAngleAxis( -pelvicAngle * DTOR, XAXIS, beta);
	//perihelion::mdpQuatFromAngleAxis( -gPelvicAngle * DTOR, XAXIS, beta);

	// Treat the center of the feet as the origin, and correct for the
	// missing hip rotation.
	perihelion::pVectorSubtract(intermediate, root, intermediate);
	//perihelion::pQuatRotateVector(beta, intermediate, intermediate);

	// Scale the point properly.
	pVector3f_SCALE(intermediate, 0.1 * height, out); 
}
void GLOblique::SkeletonToWorldConversion(perihelion::pVector3f& jointLoc) {
	float lowestAnkle;
	//perihelion::pVector3f rightAnkle, leftAnkle;

	/*currentMDP->GetJointDistalByName("Right Ankle", rightAnkle);
	currentMDP->GetJointDistalByName("Left Ankle", leftAnkle);
	HominoidToWorldConversion(leftAnkle, leftAnkle); 
	HominoidToWorldConversion(rightAnkle, rightAnkle);

	if(leftAnkle[2] < rightAnkle[2])
		lowestAnkle = leftAnkle[2]; 
	else
		lowestAnkle = rightAnkle[2];
	jointLoc[2] = jointLoc[2] + lowestAnkle;*/

	//currentMDP->GetJointProximalByName("Right Ankle", rightAnkle);
	//currentMDP->GetJointProximalByName("Left Ankle", leftAnkle);

	Skeleton* skel = workingSkeleton();
	Vector3 joint(jointLoc[0],jointLoc[1],jointLoc[2]);
	joint = joint + skel->getJoint(JT_ANKLE_CENTER, true) -
					((skel->getJoint(JT_RANKLE,true) + skel->getJoint(JT_LANKLE, true))/2);
	jointLoc[0] = joint[0];
	jointLoc[1] = joint[1];
	jointLoc[2] = joint[2];

}
bool GLOblique::AreFeetLevel(void) {
	//if(GetDocument()->getStance() != STANCE_BOTH_FEET)
	if(pDoc->getFeetSupport() != FS_Both)
		return false;

	Skeleton* skel = workingSkeleton();

	Length lLocX(0,5),lLocY(0,5),lHeelZ(0,5),rHeelZ(0,5);
	skel->C4EPoint(JT_LHEEL,lLocX,lLocY,lHeelZ);
	skel->C4EPoint(JT_RHEEL,lLocX,lLocY,rHeelZ);
	float left = lHeelZ.ValueIn(MetricCns) * 10.0; // convert to mm
	float right = rHeelZ.ValueIn(MetricCns) * 10.0; // convert to mm

	bool level = false;
	if(fabs(right - left) < 5)
		return true;
	else
		return false;
}

void GLOblique::createContextMenu(CPoint point) {
	// if a camera rotation was just completed, do not launch the context menu
	if(bigMouseMove) return;

	CMenu menu;
	menu.LoadMenu(IDR_Context_Oblique);

	switch(modelType) { 
		case MT_NONE:
			menu.CheckMenuItem(ID_MODELTYPE_NONE, MF_CHECKED | MF_BYCOMMAND);
			break;
		case MT_WIREFRAME:
			menu.CheckMenuItem(ID_MODELTYPE_WIREFRAME, MF_CHECKED | MF_BYCOMMAND);
			break;
		case MT_FLESH:
			menu.CheckMenuItem(ID_MODELTYPE_FLESH, MF_CHECKED | MF_BYCOMMAND);
			break;
	}

	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,owner);
}

void GLOblique::handleMouseMove(UINT nFlags, CPoint point) {
	if(!mouseDown || !(nFlags & (MK_LBUTTON | MK_RBUTTON))) return;

	// the threshold before true mouse movement is recorded
	const int ContextClickArea = 3;

	const double rotationSpeedVert = .70;
	const double rotationSpeedHorz = .80;
	const float distChangeRate = 8;
	const float lengthChangeRate = 1;

	CPoint Offset = CPoint(point - lastMousePoint);
	lastMousePoint = point;

	if(!bigMouseMove && (abs(point.x - initMousePoint.x) > ContextClickArea ||
		abs(point.y - initMousePoint.y) > ContextClickArea)) {
		bigMouseMove = true;
		SetCursor(NULL);
	} else if(!bigMouseMove) return;

	// perform rotation on right drag / ctrl + left drag
	if(nFlags & MK_RBUTTON || (nFlags & MK_LBUTTON && nFlags & MK_CONTROL)) {
		if(Offset.x) {
			int newAzimuth = camera.azimuth;
			newAzimuth -= int(Offset.x * rotationSpeedHorz + Offset.x / abs(Offset.x));

			while(newAzimuth > 180) newAzimuth -= 360;
			while(newAzimuth < -180) newAzimuth += 360;

			camera.azimuth = userCamera.azimuth = newAzimuth;
		}

		if(Offset.y) {
			int newAlt = camera.altitude;
			newAlt += int(Offset.y * rotationSpeedVert + Offset.y / abs(Offset.y));

			if(newAlt > 89) newAlt = 89;
			if(newAlt < -89) newAlt = -89;

			camera.altitude = newAlt;
			userCamera.altitude = newAlt;
		}
	}

	// perform zooming on shift + left button
	else if(nFlags & MK_LBUTTON && nFlags & MK_SHIFT) {
		int newDist, newLength;
		double pctToFull;

		pctToFull =	(camera.distance - MIN_CAM_DISTANCE) / (double)(MAX_CAM_DISTANCE - MIN_CAM_DISTANCE);

		// Need the "+ .5" in the following line to keep zoom effect from getting 
		// ridiculously slow and stopping at closest distance.
		newDist = int(camera.distance + (Offset.y * (pctToFull * 10 + .5) * distChangeRate));

		if(newDist < MIN_CAM_DISTANCE)
			newDist = MIN_CAM_DISTANCE;
		if(newDist > MAX_CAM_DISTANCE)
			newDist = MAX_CAM_DISTANCE;

		camera.distance = newDist;

		newLength = int(camera.focalLength + (Offset.x * lengthChangeRate));

		if(newLength < 20)
			newLength = 20;
		if(newLength > 2000)
			newLength = 2000;

		camera.focalLength = newLength;
	}

	// otherwise, pan on left drag
	else if(nFlags & MK_LBUTTON) {
		camera.horizontalOffset += Offset.x * 5;
		userCamera.horizontalOffset = camera.horizontalOffset;
		camera.verticalOffset += Offset.y * 5;
		userCamera.verticalOffset = camera.verticalOffset;
	}

	onDraw();
	pDoc->MakeDirty();
}
void GLOblique::handleMouseWheel(UINT nFlags, short zDelta, CPoint point) {
	int newDist;
	double pctToFull;

	// percentage to full
	pctToFull = double(camera.distance - MIN_CAM_DISTANCE) / double(MAX_CAM_DISTANCE - MIN_CAM_DISTANCE);

	// Need to "+ .5" to keep zoom effect from getting ridiculously slow
	//    and stopping at closest distance.
		newDist = int(camera.distance + (-zDelta * (pctToFull * 10 + .5)));

	if(newDist < MIN_CAM_DISTANCE)
		newDist = MIN_CAM_DISTANCE;			
	if(newDist > MAX_CAM_DISTANCE)
		newDist = MAX_CAM_DISTANCE;

	camera.distance = newDist;

	onDraw();
	pDoc->MakeDirty();
}
void GLOblique::handleRButtonDown(UINT nFlags, CPoint point) {
	bigMouseMove = false;
	mouseDown = true;
	lastMousePoint = initMousePoint = point;
	owner->SetCapture();
}
void GLOblique::handleRButtonUp(UINT nFlags, CPoint point) {
	SetCursor(Get_App_Ptr()->mArrowCursorHdl); 
	mouseDown = false;
	if(!(nFlags & MK_LBUTTON))
		ReleaseCapture();
}
void GLOblique::handleLButtonDown(UINT nFlags, CPoint point) {
	bigMouseMove = false;
	mouseDown = true;
	lastMousePoint = initMousePoint = point;
	owner->SetCapture();
}
void GLOblique::handleLButtonUp(UINT nFlags, CPoint point) {
	SetCursor(Get_App_Ptr()->mArrowCursorHdl); 
	mouseDown = false;
	if(!(nFlags & MK_RBUTTON))
		ReleaseCapture();
}

Camera &GLOblique::getCamera(void) {
	return camera;
}
Camera &GLOblique::getUserCamera(void) {
	return userCamera;
}
HandHeldObject &GLOblique::getHandHeld(void) {
	return handheld;
}

E_Model_Type GLOblique::getModelType(void) const {
	return modelType;
}
/*bool GLOblique::getShoesEnabled(void) const {
	return shoesEnabled;
}*/
bool GLOblique::getFloorEnabled(void) const {
	return floorEnabled;
}
void GLOblique::setModelType(E_Model_Type type) {
	modelType = type;
}
/*void GLOblique::setShoesEnabled(bool enable) {
	shoesEnabled = enable;
}*/
void GLOblique::setFloorEnabled(bool enable) {
	floorEnabled = enable;
}

LPPICTURE GLOblique::getBackground(void) const {
	return bgPicture;
}
Picture_Mode GLOblique::getBackgroundMode(void) const {
	return bgMode;
}

bool GLOblique::getBackgroundActive(void) const {
	return bgActive;
}

float GLOblique::getBackgroundAlpha(void) const {
	return bgAlpha;
}

void GLOblique::setBackground(LPPICTURE pic) {

	if(bgPicture) bgPicture->Release();
    if(bgTexture) glDeleteTextures(1, &bgTexture);
    if(!pic)
    {
        bgTexture = 0;
        bgPicture = NULL;
        bgActive = false;
        return;
    }
    bgPicture = pic;
    bgPicture->AddRef();

    // flag texture for updating once gl context is active again
    bgPostUpdate = true;

}



void GLOblique::setBackgroundMode(Picture_Mode mode) {
	bgMode = mode;
}

void GLOblique::setBackgroundActive(bool bg) {
	bgActive = bg;
}
void GLOblique::setBackgroundAlpha(float alpha) {
	bgAlpha = alpha;
}

void GLOblique::setDefaultColors() {
	backgroundColor[0] = 255;
	backgroundColor[1] = 255;
	backgroundColor[2] = 145;

	skinColor[0] = 169;
	skinColor[1] = 128;
	skinColor[2] = 90;

	maleShirtColor[0] = 0;
	maleShirtColor[1] = 108;
	maleShirtColor[2] = 54;

	malePantsColor[0] = 0;
	malePantsColor[1] = 64;
	malePantsColor[2] = 128;

	femaleShirtColor[0] = 255;
	femaleShirtColor[1] = 128;
	femaleShirtColor[2] = 128;

	femalePantsColor[0] = 0;
	femalePantsColor[1] = 64;
	femalePantsColor[2] = 128;

	handHeldColor[0] = 169;
	handHeldColor[1] = 128;
	handHeldColor[2] = 90;

	handHeldOutlineColor[0] = 0;
	handHeldOutlineColor[1] = 0;
	handHeldOutlineColor[2] = 0;
}

unsigned char GLOblique::backgroundColor[3] = {255,255,145};
unsigned char GLOblique::skinColor[3] = {169,128,90};
unsigned char* GLOblique::shirtColor;
unsigned char* GLOblique::pantsColor;
unsigned char GLOblique::maleShirtColor[3] = {0,108,54};
unsigned char GLOblique::malePantsColor[3] = {0,64,128};
unsigned char GLOblique::femaleShirtColor[3] = {255,128,128};
unsigned char GLOblique::femalePantsColor[3] = {0,64,128};
unsigned char GLOblique::handHeldColor[3] = {169,128,90};
unsigned char GLOblique::handHeldOutlineColor[3] = {0,0,0};

double GLOblique::shouldDisp = 0;
Vector3 GLOblique::motionvecs[100][32];