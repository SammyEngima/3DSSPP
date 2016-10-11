#pragma once

// Angle Cues
/* In the body segment angle dialog box (dynamic mode only right now), when the user puts focus
 * on a box, or subsequently changes the angle value, a "Cue" is drawn indicating the angle of interest.
 * The following is the structural design of this system:
 * the dialog box sets up an AngleCueData struct with the appropriate information about the angle to be drawn.
 * It calls C_Hom_Doc::cueAngle() with this struct to state that it should be shown.
 * C_Hom_Doc::cueAngle() simply passes this message on to the windows by calling CGLView::cueAngle() with the same struct.
 * Note that this is why cueAngle() is in CGLView and is virtual, because the document stores the views as pointers
 * to CGLViews. The CGLView::cueAngle() does nothing so the oblique view, which does not override cueAngle, does nothing with cues.
 * CGLStickView has as protected member variables, an AngleCueData, and a map from AngleTypes to member functions
 * that set up angle cues to be drawn for that AngleType using the AngleCueData info.
 * The ortho views, child classes of CGLStickView, put an entry in the map for each AngleType they want to display.
 * e.g. top view shows horizontal angles so it does themap[AT_LimbHL] = themap[AT_LimbHR] = &CGLStickVew::CueLimbH;
 * So CGLStickView overrides cueAngle to store the AngleCueData it is passed, and 
 * check the map for an entry for that AngleType,
 * and calls the function if it is found.  otherwise it calls cancelCue() which causes no cue to be shown.
 * In draw(), CGLStickView calls drawCue if CueEnabled is true. drawCue() draws the cue based on the info
 * that the mapped functions set up.
 */
enum JointID;
enum E_Angles;
enum AngleType {AT_LimbVL, AT_LimbVR, AT_LimbHL, AT_LimbHR,
				AT_TrunkLB, AT_TrunkF, AT_TrunkAR,
				AT_HeadLB, AT_HeadF, AT_HeadAR,
				AT_PelvisLB, AT_Pelvis,
				AT_HandRot,
				AT_ClavicleVL, AT_ClavicleVR, AT_ClavicleHL, AT_ClavicleHR,
				AT_HorizontalPosNeg,
				NUM_ANGLE_TYPES};
struct AngleCueData {
	AngleCueData();
	AngleCueData(E_Angles AI, JointID P, JointID D, AngleType AT, E_Angles OI = (E_Angles)-1);
	// angleID is the angle we are drawing
	// otherID is an angle we might need for calculations. this varies depending on what type of angle we're cueing.
	E_Angles angleID, otherID;
	// proximal is the joint which the angle is around. distal is the far side of the segment of concern
	JointID proximal, distal;
	AngleType angleType;

	static AngleCueData Build(E_Angles AI);
};
