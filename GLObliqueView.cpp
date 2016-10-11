#include "stdafx.h"
#include "GLObliqueView.h"
//#include "hom.h"
//#include "afxwin.h"
#include "resource.h"
#include "hom_doc.h"
#include "GLOblique.h"

BEGIN_MESSAGE_MAP(CGLObliqueView, CGLView)
    //{{AFX_MSG_MAP(CGLObliqueView)
    ON_COMMAND(ID_Obl_FrontView, OnOblFrontView)
    ON_COMMAND(ID_Obl_StdZoom, OnOblStdZoom)
    ON_COMMAND(ID_Obl_BackView, OnOblBackView)
    ON_COMMAND(ID_Obl_LeftView, OnOblLeftView)
    ON_COMMAND(ID_Obl_RightView, OnOblRightView)
    ON_COMMAND(ID_Obl_TopView, OnOblTopView)
    ON_COMMAND(ID_Obl_CenterFigure, OnOblCenterFigure)
    ON_COMMAND(ID_MODELTYPE_NONE, OnModeltypeNone)
    ON_COMMAND(ID_MODELTYPE_WIREFRAME, OnModeltypeWireframe)
    ON_COMMAND(ID_MODELTYPE_FLESH, OnModeltypeFlesh)
    ON_COMMAND(ID_Obl_UserView, OnOblUserview)
    //}}AFX_MSG_MAP(CGLObliqueView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CGLObliqueView, CGLView)

CGLObliqueView::CGLObliqueView(void)
{
	glInteractive = new GLOblique(this);
}

CGLObliqueView::~CGLObliqueView(void)
{
}

void CGLObliqueView::OnOblFrontView(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();
    camera.horizontalOffset = 0;
    camera.verticalOffset = 0;
    camera.altitude = 0;
    camera.azimuth = 0;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnOblStdZoom(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();
    camera.focalLength = 50;
    camera.distance = 2290;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnOblBackView(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();
    camera.horizontalOffset = 0;
    camera.verticalOffset = 0;
    camera.altitude = 0;
    camera.azimuth = 180;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnOblLeftView(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();
    camera.horizontalOffset = 0;
    camera.verticalOffset = 0;
    camera.altitude = 0;
    camera.azimuth = 90;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnOblRightView(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();
    camera.horizontalOffset = 0;
    camera.verticalOffset = 0;
    camera.altitude = 0;
    camera.azimuth = -90;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnOblTopView(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();
    camera.horizontalOffset = 0;
    camera.verticalOffset = 0;
    camera.altitude = 89;
    camera.azimuth = 0;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}


void CGLObliqueView::OnOblUserview(void)
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();

	camera = ((GLOblique*)glInteractive)->getUserCamera();

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnOblCenterFigure(void) 
{
	Camera& camera = ((GLOblique*)glInteractive)->getCamera();

    camera.horizontalOffset = 0;
    camera.verticalOffset = 0;

	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnModeltypeNone(void)
{
	((GLOblique*)glInteractive)->setModelType(MT_NONE);
	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnModeltypeWireframe(void)
{
	((GLOblique*)glInteractive)->setModelType(MT_WIREFRAME);
	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnModeltypeFlesh(void)
{
	((GLOblique*)glInteractive)->setModelType(MT_FLESH);
	this->OnDraw(0);
    GetDocument()->MakeDirty();
}

void CGLObliqueView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	this->OnDraw(0);
}

void CGLObliqueView::updateMDP() {
	((GLOblique*)glInteractive)->updateMDP();
}

Camera &CGLObliqueView::getCamera(void) {
	return ((GLOblique*)glInteractive)->getCamera();
}

Camera &CGLObliqueView::getUserCamera(void) {
	return ((GLOblique*)glInteractive)->getUserCamera();
}

HandHeldObject &CGLObliqueView::getHandHeld(void) {
	return ((GLOblique*)glInteractive)->getHandHeld();
}

E_Model_Type CGLObliqueView::getModelType(void) const {
	return ((GLOblique*)glInteractive)->getModelType();
}
/*bool CGLObliqueView::getShoesEnabled(void) const {
	return ((GLOblique*)glInteractive)->getShoesEnabled();
}*/
bool CGLObliqueView::getFloorEnabled(void) const {
	return ((GLOblique*)glInteractive)->getFloorEnabled();
}
void CGLObliqueView::setModelType(E_Model_Type type) {
	((GLOblique*)glInteractive)->setModelType(type);
}
/*void CGLObliqueView::setShoesEnabled(bool enable) {
	((GLOblique*)glInteractive)->setShoesEnabled(enable);
}*/
void CGLObliqueView::setFloorEnabled(bool enable) {
	((GLOblique*)glInteractive)->setFloorEnabled(enable);
}

LPPICTURE CGLObliqueView::getBackground(void) const {
	return ((GLOblique*)glInteractive)->getBackground();
}
Picture_Mode CGLObliqueView::getBackgroundMode(void) const {
	return ((GLOblique*)glInteractive)->getBackgroundMode();
}
bool CGLObliqueView::getBackgroundActive(void) const {
	return ((GLOblique*)glInteractive)->getBackgroundActive();
}
float CGLObliqueView::getBackgroundAlpha(void) const {
	return ((GLOblique*)glInteractive)->getBackgroundAlpha();
}
void CGLObliqueView::setBackground(LPPICTURE pic) {
	((GLOblique*)glInteractive)->setBackground(pic);
}
void CGLObliqueView::setBackgroundMode(Picture_Mode mode) {
	((GLOblique*)glInteractive)->setBackgroundMode(mode);
}
void CGLObliqueView::setBackgroundActive(bool bg) {
	((GLOblique*)glInteractive)->setBackgroundActive(bg);
}
void CGLObliqueView::setBackgroundAlpha(float alpha) {
	((GLOblique*)glInteractive)->setBackgroundAlpha(alpha);
}