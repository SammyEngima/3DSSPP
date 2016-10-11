#if ! defined( BatchFileHpp )
#define        BatchFileHpp 


#include <map>

#include <string>//"String.hpp"
#include "Dg_BatchFile.h"


class C_Hom_Doc;

class BatchFile
{
public:
	BatchFile(	const std::string& aFilePath,
				const std::string& aFileRoot,
					C_Hom_Doc* aDocPtr	);

	bool Process();

private:
	bool mAutoExport;

	std::string mInFileName;
	std::string mExportFileName;
	std::string mLogFileName;

	C_Hom_Doc* mDocPtr;

	Dg_BatchFile mProgressDialog;

	enum cBatchCommand
	{
		bch3DSSPPBatchFile,
		bchDescription,
		bchAnthropomentry,
		bchJointAngles,
		bchJointAnglesWithLegs,	// includes horizontal and vertical leg angles specified in 6.x definitions
		bchJointAnglesAll, // includes all joint angles
		bchHandLoads,
		bchComment,
		bchExport,
		bchAutoExport,
		bchPause,
		bchSupport, //support selection
		// TODO adding frame change commands for Lauren's climbing study. maybe for real if Chuck likes it
		bchFrameChange,
		bchPosturePrediction,
		bchLocations,
		bchexternala, //right elbow
		bchexternalb, //right shoulder
		bchexternalc, //left elbow
		bchexternald, //left shoulder
		bchexternale, //low back
		bchexternalf, //right hip
		bchexternalg, //right knee
		bchexternalh, //right ankle
		bchexternali, //left hip
		bchexternalj, //left knee
		bchexternalk, //left ankle
		bchexternall, //right wrist
		bchexternalm, //left wrist
		bchexternaln, //head
		bchLeftHandComponentLoad, //left hand component force
		bchRightHandComponentLoad //Right hand component force
	};

	typedef std::map< std::string, cBatchCommand > cMapType;

	cMapType mCommands;
};
#endif
