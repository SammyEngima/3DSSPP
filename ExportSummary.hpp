#ifndef ExportSummaryHppMcr
#define ExportSummaryHppMcr

#include <fstream>

class C_Hom_Doc;

enum E_Delimiter_Mode { DM_COMMA, DM_TAB };
enum E_File_Mode { FM_APPEND, FM_OVERWRITE };

class ExportSummary
{
    public:
	    ExportSummary(C_Hom_Doc *aDocumentCPtr);

        void Update();

        BOOL Overwrite() const
	    {
		    return mOverwrite;
	    }

        BOOL Export(std::fstream &aOutputFile);
        void Export(std::fstream &aOutputFile, E_Delimiter_Mode aDelimiterMode, E_File_Mode aFileMode);
		BOOL Export(std::fstream &aOutputFile, int frameid);
    private:

        void Write(const char *aCharPtr);
        void Write(int aInt);
        void Write(float aFloat);
        void Write(double aDouble);
		void EndL();

        friend class Dg_ExportOptions;

        C_Hom_Doc *mDocPtr;
        std::fstream *mFileStreamPtr;

		// state variable to know if the next data written is at the beginning of a line
		// if it is, we don't write a delimiter before the data
		bool newLine;
           
        int   mFileMode;
	    BOOL  mOverwrite;

	    BOOL mBodySegmentAngles;
	    BOOL mHandForces;
	    BOOL mJointLocations;
	    BOOL mJointForces;
	    BOOL mJointMoments;
	    BOOL mPostureAngles;
	    BOOL mStrength;
	    BOOL mSummaryResults;
	    BOOL mLowBack;
        BOOL mFatigueReport;
		BOOL mBalanceReport;
		BOOL mShoulderGeometry;
		BOOL mShoulderAnalysis;

		BOOL mColumnHeaders;

		int mDelimiterMode;
		char mDelimiter;
};

#endif
