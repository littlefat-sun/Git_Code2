#include "rayselecttool.h"
#include "mathengin/ev_math.h"
#include "graphic/viewport.h"
#include "spatial3dlayer/featuregrouplayer.h"
#include "spatial3dengine/globecamera.h"
#include "spatial3dengine/scene.h"
#include "globecontrol/globecontrol.h"
#include "globecontrol/globecontrolmanager.h"
#include "spatial3dcalculator/spatialcalculator.h"
#include "spatialdatabase/queryfilter.h"
#include "spatialinterface/ilayerselection.h"
#include "spatial3dlayer/vectordbclampscenelayer.h"
#include "spatialobject/geometry/point.h"
using namespace EarthView::World::Spatial::Math;
using namespace EarthView::World::Spatial3D::Controls;
using namespace EarthView::World::Spatial3D::Atlas;
using namespace EarthView::World::Spatial::Atlas;
using namespace EarthView::World::Core;
using namespace EarthView::World::Graphic;
using namespace EarthView::World::Spatial3D::Utility;
using namespace EarthView::World::Spatial::SystemUI;
using namespace EarthView::World::Spatial::Geometry;
using namespace EarthView::World::Spatial::GeoDataset;
using namespace EarthView::World::Spatial::Utility;
namespace EarthView
{
	namespace World
	{
		namespace Spatial3D
		{
			namespace SystemUI
			{
				REGISTER_FACTORY_CLASS(CRaySelectTool);
				void CRaySelectTool::CObjectListener::objectDestroyed(CMovableObject* pMovableObject)
				{
					CMutexLocker locker(&(mParent->EV_AUTO_MUTEX_NAME));
					ev_bool changed = false;
					RaySceneQueryResult& result = mParent->mSelector.getLastResults();
					for (RaySceneQueryResult::iterator iter = result.begin();
						iter != result.end();++iter)
					{
						   RaySceneQueryResultEntry& entry = *iter;
						   if (entry.movable == pMovableObject)
						   {
							   result.erase(iter);
							   if (mParent->mPickingMovable && mParent->mPickingMovable->movable == pMovableObject)
							   {
									mParent->mPickingMovable = NULL;
							   }
							   changed = true;
							   break;
						   }
					}
					if (changed && mParent->mControl && CGlobeControlManager::getSingletonPtr()->existGlobeControl(mParent->mControl))
					{
					   mParent->mControl->setSelection(result);
					}
				}
				void CRaySelectTool::CSelectionListener::onSelectionChanged(CGlobeControl& globeControl,CGlobeSelection& result)
				{
					//mParent->resetResults();
				}
				void CRaySelectTool::CSelectionListener::onSelectionChanged(CGlobeControl* globeControl,RaySceneQuery& result)
				{
					if (result.size() == 0)
					{
						mParent->resetResults();
						//mParent->clearLayerSelection();
					}
				}
				void CRaySelectTool::CSelectionListener::onSelectionChanged(CGlobeControl* globeControl,SceneQuery& result)
				{
					 if (result.movables.size() == 0&&result.movableEx.size() == 0)
					 {
						 //	 mParent->clearLayerSelection();
					 }
				}
				CRaySelectTool::CRaySelectTool(CGlobeControl* ctrl)
					:mControl(ctrl)
					,mDownX(0)
					,mDownY(0)
					,mDraging(false)
					,mPickingMovable(NULL)
					,mSelector(mControl->getSceneManager())
					,mTrackingType(CEditBoundingBox::TT_Unknown)
					,mDoubleClicked(false)
					,mSelectButton(CGUIEvent::LEFT_MOUSE_BUTTON)
					,mEditable(true)
					,mWgsSR(NULL)
				{
					mSelector.setSortByDistance(true,1);
					mpObjectListener = new CObjectListener(this);
					mpSelectionListener = new CSelectionListener(this);
					mControl->addQueryListener(mpSelectionListener);
				}
				CRaySelectTool::CRaySelectTool(CNameValuePairList* pList)
					:mControl((CGlobeControl*)pList->GetAt("ref_ctrl"))
					,mDownX(0)
					,mDownY(0)
					,mDraging(false)
					,mPickingMovable(NULL)
					,mSelector(mControl->getSceneManager())
					,mTrackingType(CEditBoundingBox::TT_Unknown)
					,mDoubleClicked(false)
					,mSelectButton(CGUIEvent::LEFT_MOUSE_BUTTON)
					,mEditable(true)
					,mWgsSR(NULL)
				{
					mSelector.setSortByDistance(true,1);
					mpObjectListener = new CObjectListener(this);
					mpSelectionListener = new CSelectionListener(this);
					mControl->addQueryListener(mpSelectionListener);
				}
				CRaySelectTool::~CRaySelectTool()
				{
					if (mSelector.getLastResults().size()>0)
					{
						resetResults();
						RaySceneQueryResult result;
						mControl->fireSelectionChanged(result);
					}
				    if (mControl&&CGlobeControlManager::getSingletonPtr()->exitGlobeControl(mControl))
				    {
						mControl->removeQueryListener(mpSelectionListener);
				    }
					delete mpSelectionListener;
					delete mpObjectListener;
					if (mWgsSR)
					{
						CCoordinateSystemFactory::releaseCoordSys(mWgsSR);
						mWgsSR = NULL;
					}
				}
				ev_void CRaySelectTool::onClicked()
				{
					mControl->setCurrentTool(this);
				}
				ev_bool CRaySelectTool::getChecked() const
				{
					return mControl->getCurrentTool() == this;
				}
				ev_bool CRaySelectTool::getEnabled() const
				{
					return mControl->getScene()!=NULL;
				}
				ev_bool CRaySelectTool::getEditable() const
				{
					return mEditable;
				}
				void CRaySelectTool::setEditable(ev_bool editable)
				{
					 mEditable = editable;
					 
					 EV_LOCK_AUTO_MUTEX;
					 if (!mEditable&&isEditing())
					 {
						 mPickingMovable->movable->endEditing();
					 }
				}
				void CRaySelectTool::setSortByDistance(ev_bool sort)
				{
					mSelector.setSortByDistance(sort);
				}
				void CRaySelectTool::setSortByDistance(ev_bool sort,ev_uint16 maxresults)
				{
					mSelector.setSortByDistance(sort,maxresults);
				}
				ev_bool CRaySelectTool::getSortByDistance() const
				{
					return mSelector.getSortByDistance();
				}
				ev_uint16 CRaySelectTool::getMaxResults() const
				{
					return mSelector.getMaxResults();
				}
				void CRaySelectTool::resetResults()
				{
					EV_LOCK_AUTO_MUTEX;

					mPickingMovable = NULL;
					RaySceneQueryResult& result = mSelector.getLastResults();
					for (RaySceneQueryResult::iterator iter = result.begin();
						iter != result.end();++iter)
					{
						RaySceneQueryResultEntry& entry = *iter;
						entry.movable->removeListener(mpObjectListener);
					}
					mSelector.clearResults();
				}
				ev_void CRaySelectTool::clearLayerSelection()
				{
					CScene* scene = dynamic_cast<CScene*>(mControl->getScene());
					if (scene)
					{
						CGeoSceneManager* sceneManager = scene->getGeoSceneManager();
						ILayer* layer = sceneManager->getFeatureGroupLayer();
						layer->clearSelection();
					}
				}
				ev_uint32 CRaySelectTool::getType() const
				{
					return TT_RaySelect;
				}
				ev_bool CRaySelectTool::pickEditableObjectFromResult(const CRay& ray)
				{
					EV_LOCK_AUTO_MUTEX;

					Real dist = -1.0;
					mPickingMovable = NULL;

					CViewport* vp = mControl->getViewport();
					RaySceneQueryResult& result = mSelector.getLastResults();
					for (RaySceneQueryResult::iterator iter = result.begin();
						iter != result.end();++iter)
					{
					   RaySceneQueryResultEntry& entry = *iter;
					   ev_int32 objIndex = -1;
					   CVector3 point;
					   if (entry.movable->selectBy(ray,vp,false,objIndex,point)&&objIndex==entry.objectIndex)
					   {
						   Real d = ray.getOrigin().distance(point);
						   if (dist<0|| d<dist)
						   {
							   mPickingMovable = &entry;
							   dist = d;
						   }
					   }
					}
					if (mPickingMovable != NULL && mPickingMovable->movable->getEditable())
					{
						return true;
					}
					else
					{
						mPickingMovable = NULL;
						return false;
					}
				}
				CVector3 CRaySelectTool::inverseProject(const CVector3& position)
				{
					ev_int32 left = mControl->getViewport()->getActualLeft();
					ev_int32 top = mControl->getViewport()->getActualTop();
					ev_int32 width = mControl->getViewport()->getActualWidth();
					ev_int32 height = mControl->getViewport()->getActualHeight();
					CMatrix4 viewportMatrix = CMatrix4::ZERO;
					viewportMatrix[0][0]= width*0.5;
					viewportMatrix[0][3]= left+width*0.5;
					viewportMatrix[1][1]= -height*0.5;
					viewportMatrix[1][3]= height*0.5+top;
					viewportMatrix[3][3]= 1.0;
					viewportMatrix[2][2]= -0.5;
					viewportMatrix[2][3]= 0.5;
					const CMatrix4& prjMatrix = mControl->getCamera()->getProjectionMatrix();
					const CMatrix4& viewMatrix = mControl->getCamera()->getViewMatrix(false);
					CMatrix4 matrix = viewportMatrix * prjMatrix * viewMatrix;
					CVector3 pixel = matrix * position;
					pixel.z = 0.0;
					return pixel;
				}
				ev_bool CRaySelectTool::isEditing() const
				{
					if (mPickingMovable && mPickingMovable->movable && mPickingMovable->movable->getEditBoundingBox())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				ev_void CRaySelectTool::updateSelectdObjectTo(ev_int32 key,ev_int32 x,ev_int32 y,IGlobeQueryListener::MouseOpType type)
				{
					CEditBoundingBox* box = mPickingMovable->getEditBoundingBox();
					if (NULL==box)
					{	return;	}
					CViewPort* vp= mControl->getViewPort();
					CVector3 center=mDownMatrix * box->getBoundingBox().getCenter();
					CVector3 piexl = inverseProject(center);
					CMatrix4* matrix;
					if(mPickingMovable->movable->getSelecteObjectWorldMatrix(mPickingMovable->objectIndex,)
					{
						if(type==IGlobeQueryListener::MOT_MouseDown)
						{
							mControl->fireSelectedObjectMoved(*mPickingMovable,matrix,type);
							return;
						}
						if(mTrackingType==CEditBoundingBox::TT_WithinBox)//平移
						{
							CRay downRay= ((CGlobeCamera*)mControl->getCamera())->screenToScreen(
								mDownX,mDownY,
								vp->getActualTop(),vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
							downRay.devide(mDownMatrix);
							IntersectResPair rp= downRay.intersects(box->getBoundingBox());
							CVector3 downPt = downRay.getPoint(rp.second);
							downPt= mDownMatrix* downPt;//鼠标点在包围盒上的位置
							CVector3 pos,scale;
							CQuaternion q;
							Real lon = 0.0,lat= 0.0,alt= 0.0;
							CSpatialCalculator::decomposeWorldMatrix(mDownMatrix,lat,lon,alt,scale,q);
							pos = CSpatialCalculator::sphericalToCartesian(lat,lon,EV_EARTHRADIUS + alt);
							Real length =downPt.length();
							CSphere sphere(CVector3(),length);
							CRay ray = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(
								x,y,
								vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
							IntersectPair pair = ray.intersects(sphere);
							if (pair.first)
							{
								CVector3 upPt=rya.getPoint(pair.second);//pt为鼠标点的新坐标
								CVector3 newPos = pos = upPt -downPt;
								newPos = CSpatialCalculator::cartesianToSpherical(newPos);
								lon = newPos.z*CMath::fRad2Deg;
								lat = newPos.y*CMath::fRad2Deg;
								alt = newPos.x-EV_EARTHRADIUS;
								CMatrix4* mat= CSpatialCalculator::composeWorldMatrix(lat,lon,alt,scale,q);
								if (mPickingMovable->movable->setSelectedObjectWorldMatrix(mPickingMovable->objectindex,mat))
								{
									mControl->fireSelectedObjectMoved(*mPickingMovable,mat,type);
								}
							}
						}
						else if(mTrackingType != CEditBoundingBox::TT_Unknown)
						{
							CMatrix4 mat1; mat1.setTrans(-box->getBoundingBox().getCenters());
							CMatrix4 mat2; mat2.setTrans(box->getBoundingBox().getCenter());
							CMatrix4 mat4;
							if ( mTrackingType ==CEditBoundingBox::TT_TouchArrowX
								|| mTrackingType == CEditBoundingBox::TT_TouchArrowY
								|| mTrackingType == CEditBoundingBox::TT_TouchArrowZ)//缩放
							{
								CVector3 down(mDownX,mDownY,0.0);
								CVector3 up(x,y,0.0);
								CVector3 v1 = down - piexl;
								CVector3 v2 = up- piexl;
								CRadian radian(CMath::ATan2(v2.y,v2.x)-CMath::ATan2(v1.y,v1.x));
								double len1 = down.distance(piexl);
								double len2 = up.distance(piexl)* CMath::Cos(radian);
								double scale = len2/len1;
								CMatrix4 t;
								if (key==CGUIEvent::MODKEY_LEFT_CTRL || CGUIEvent::MODKEY_RIGHT_CTRL)
								{
									if (mTrackingType == CEditBoundingBox::TT_TouchArrowX)
									{
										t.setScale(CVector3(scale,1.0,1.0));
									}
									else if (mTrackingType == CEditBoundingBox::TT_TouchArrowY)
									{
										t.setScale(CVector3(1.0,scale,1.0));
									}
									else if (mTrackingType == CEditBoundingBox::TT_TouchArrowZ)
									{
										t.setScale(CVector3(1.0,1.0,scale));
									}
								}
								else
								{
									t.setScale(CVector3(scale,scale,scale));
								}
								mat = mDownMatrix* mat2* t*mat1;

							}
							else if (mTrackingType ==CEditBoundingBox::TT_OnCircleX
								|| mTrackingType == CEditBoundingBox::TT_OnCircleY
								|| mTrackingType == CEditBoundingBox::TT_OnCircleZ))//旋转
							{

								CRay downRay = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(
									mDownX,mDownY,
									vp->getActualTop(),vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
								downRay.devide(mDownMatrix);

								CRay nowRay = ((CGlobeCamera*)mControl->getCamera())->screenToScene(
									x,y,
									vp->getActualtTop(),vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
								nowRay.devide(mDownMatrix);
								CQuaternion t;
								if (mTrackingType ==CEditBoundingBox::TT_OnCircleX)
								{
									CPlane plane(CVector3::UNIT_X,box->getBoundingBox().getCenter());//YOZ平面
									CVector3 downPt = downRay.getPoint(downRay.intersects(plane).second);
									CVector3 nowPt = nowRay.getPoint(nowRay.intersects(plane).second);
									CVector3 downDir = (downPt - box->getBoundingBox().getCenter()).normalisedCopy();
									CVector3 nowDir = (nowPt -box->getBoundingBox().getCenter()).normalisedCopy();
									t= downDir.getRotationTo(nowDir);
								}
								else if (mTrackingType == CEditBoundingBox::TT_OnCircleY)
								{
									CPlane plane(CVector3::UNIT_Y,box->getBoundingBox().getCenter());//XOZ平面
									CVector3 downPt = downRay.getPoint(downRay.intersects(plane).second);
									CVector3 nowPt = nowRay.getPoint(nowRay.intersects(plane).second);
									CVector3 downDir = (downPt - box->getBoundingBox().getCenter()).normalisedCopy();
									CVector3 nowDir = (nowPt -box->getBoundingBox().getCenter()).normalisedCopy();
									t= downDir.getRotationTo(nowDir);
								}
								else if (mTrackingType == CEditBoundingBox::TT_OnCircleZ)
								{
									CPlane plane(CVector3::UNIT_Z,box->getBoundingBox().getCenter());//XOY平面
									CVector3 downPt = downRay.getPoint(downRay.intersects(plane).second);
									CVector3 nowPt = nowRay.getPoint(nowRay.intersects(plane).second);
									CVector3 downDir = (downPt - box->getBoundingBox().getCenter()).normalisedCopy();
									CVector3 nowDir = (nowPt -box->getBoundingBox().getCenter()).normalisedCopy();
									t= downDir.getRotationTo(nowDir);
								}
								mat= mDownMatrix* mat2*CMatrix4(t)* mat1;
							}
							else if (mTrackingType ==CEditBoundingBox::TT_OnCircleX
								|| mTrackingType == CEditBoundingBox::TT_OnCircleY
								|| mTrackingType == CEditBoundingBox::TT_OnCircleZ))//平移
							{
								CRay downRay = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(
									mDownX,mDownY,
									vp->getActualTop(),vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
								downRay.devide(mDownMatrix);
								CRay nowRay = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(
									x,y,
									vp->getActualTop(),vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
								nowRay.devide(mDownMatrix);

								CVector3 camDir = mControl->getCamera()->getDirection();
								camDir.normalise();
								CVector3 offset;
								if (mTrackingType == CEditBoundingBox::TT_OnAxisX)
								{
									CVector3 norm1 = CVector3::UNIT_Y;//备选法线
									CVector3 norm2 = CVectro::UNIT_Z;
									double angle1 = CMath::ACos(norm1.dotProduct(camDir)).valueDegrees();
									double angle2 = CMath::ACos(norm2.dotProduct(camDir)).valueDegrees();
									CVector3 norm =CMath::Abs(angle1)<CMath::Abs(angle2)? norm1:norm2;
									CPlane plane(norm,box->getBoundingBox().getCenter());
									CVector3 downPt = downRay.getPoint(downRay.intersects(plane).second);
									downPt.y = downPt.z=0.0;
									CVector3 nowPt = nowRay.getPoint(nowRay.intersects(plane).second);
									nowRay.y=nowRay.z=0.0;
									offset =nowPt-downPt;
								}
								else if (mTrackingType == CEditBoundingBox::TT_OnAxisY)
								{
									 CVector3 norm1 = CVector3::UNIT_X;//备选法线
									 CVector3 norm2 = CVector3::UNIT_Z;
									 double angle1 = CMath::ACos(norm1.dotProduct(camDir)).valueDegress();
									 double angle2 = CMaht::ACos(norm2.dotProduct(camDir)).valueDegress();
									 CVector3 norm = CMath::Abs(angle1)<CMath::Abs(angle2)?norm1:norm2;
									 CPlane plane(norm,box->getBoundingBox().getCenter());
									 CVector3 downPt = downRay.getPoint(downRay.intersects(plane).second);
									 downPt.x = downPt.z = 0.0;
									 CVector3 nowPt = nowRay.getPoint(nowRay.intersects(plane).second);
									 nowPt.x = nowPt.z = 0.0;
									 offset = nowPt - downPt;
								}
								else if (mTrackingType == CEditBoundingBox::TT_OnAxisZ)
								{													   
									CVector3 norm1 = CVector3::UNIT_Y;//备选法线
									CVector3 norm2 = CVector3::UNIT_X;
									double angle1 = CMath::ACos(norm1.dotProduct(camDir)).valueDegress();
									double angle2 = CMath::ACos(norm2.dotProduct(camDir)).valueDegress();
									CVector3 norm = CMath::Abs(angle1)<CMath::Abs(angle2)?norm1:norm2;
									CPlane plane(norm,box->getBoundingBox().getCenter());
									CVector3 downPt = downRay.getPoint(downRay.intersects(plane).second);
									downPt.y = downPt.x =0.0;
									CVector3 nowPt = nowRay.getPoint(nowRay.intersects(plane).second);
									nowPt.y = nowPt.x = 0.0;
									offset = nowPt - downPt;
								}

								CMatrix4 offsetMatrix = CMatrix4::getTrans(offset);
								CMatrix4 trueMatrix = mDownMatrix* offsetMatrix;
								CVector3 nowPos = trueMatrix.getTrans();
								Real lon=0.0;lat=0.0;alt=0.0;
								CVector3 scale;
								CQuaternion q;
								CSpatialCalculator::decomposeWorldMatrix(mDownMatrix,lat,lon,alt,scale,q);
								CVector3 p = CSpatialCalculator::cartesianToSpherical(nowPos);
								lon = p.x*CMath::fRad2Deg;
								lat = p.y*CMath::fRad2Deg;
								alt = p.z- EV_EARTHRADIUS;
								mat = CSpatialCalculator::composeWorldMatrix(lat,lon,alt,scale,q);
							}
							if (mPickingMovable->movable->setSelectedObjectWorldMatrix(mPickingMovable->objectIndex,mat))
							{
								mControl->fireSelectedObjectMoved(*mPickingMovable,mat,type);
							}
						}
					}
				}
				ev_bool CRaySelectTool::onMouseDown(ev_int32 button,ev_int32 shift,ev_int32 x,ev_int32 y)
				{
					if (mEditable&& button == CGUIEvent::LEFT_MOUSE_BUTTON||button ==mSelectButton)
					{
						mDownX = x;
						mDownY = y;
						mDownTime = CTime::currentTime();
						if (mPickingMovable&&button == CGUIEvent::LEFT_MOUSE_BUTTON)
						{
							updateSelectdObjectTo(shift,mDownX,mDownY,IGlobeQueryListener::MOT_MouseDown);
							mPickingMovable->movable->getSelectedObjectWorldMatrix(mPickingMovable->objectIndex,mDownMatrix);
						}
						if (!mDoubleClicked&&button == CGUIEvent::LEFT_MOUSE_BUTTON)
							mDraging = true;
						CViewport* vp =mControl->getViewport();
						CRay ray = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(x,y,vp->getActualTop(),vp->getActualLeft(),
							vp->getActualWidth(),vp->getActualHeight());
						if (isEditing()==false&&button==CGUIEvent::LEFT_MOUSE_BUTTON)
						{
							if (pickEditableObjectFromResult(ray)&&mPickingMovable->movable->getEditBoundingBox())
								return true;
						}
						else if (button==CGUIEvent::LEFT_MOUSE_BUTTON)
						{
							mTrackingType = CEditBoundingBox::TT_Unknown;
							if (mPickingMovable)
							{
								CEditBoundingBox* box = mPickingMovable->movable->getEditBoundingBox();
								if (box)
								{
									mTrackingType = box->trackRay(ray);
									if (mTrackingType!=CEditBoundingBox::TT_Unkonwn)
										return true;
									else
									{
										mDraging = true;
									}
								}
							}
						}
					}
					return false;
				}
				ev_bool CRaySelectTool::onDoubleClick(ev_int32 button,ev_int32 shift,ev_int32 x,ev_int32 y)
				{
					if (button!=CGUIEvetn::LEFT_MOUSE_BUTTON||!mEditable)
						return false;
					mDoubleClicked = false;
					onMouseDown(button,shift,x,y);
					if (mSelectButton != CGUIEvent::LEFT_MOUSE_BUTTON)
					{
						mDoubleClicked =false;
						onMouseUp(mSelectButton,shift,x,y);
						mDoubleClicked =true;
						onMouseDown(CGUIEvent::LEFT_MOUSE_BUTTON,shift,x,y);
					}
					if (mPickingMovable&&mPickingMovable->movable)
					{
						if (isEditing()==false)
						{
							mPickingMovable->movable->startEditing(mPickingMovable->objectIndex);
							return true;
						}
						else
						{
							mPickingMovable->movable->endEditing();
							return true;
						}
					}
					return mSelector.getLastResults().size();
				}
				ev_bool CRaySelectTool::onMouseMove(ev_int32 button,ev_int32 shift,ev_int32 x,ev_int32 y)
				{
					if (mEditable&&isEditing()&&(button==0||button==CGUIEvent::LEFT_MOUSE_BUTTON))
					{
						if (mDraging&&!mDoubleClicked)
						{
							if (mDownTime.msecsTo(CTime::currentTime())>200)
							{
								updateSelectdObjectTo(shift,x,y,IGlobeQueryListener::MOT_MouseMove);
							}
							return true;
						}
						else
						{
							//根据鼠标位置，改变包围盒局部的颜色
							CEditBoundingBox* box= mPickingMovable->movable->getEditBoundingBox();
							if (box)
							{
								CViewport* vp= mControl->getaViewport();
								CRay ray = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(x,y,vp->getActualTop(),
									vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
								return box->trackRay(ray) != CEditBoundingBox::TT_Unknown;
							}
						}
					}
					return false;
				}
				ev_bool CRaySelectTool::onMouseUp(ev_int32 button,ev_int32 shift,ev_int32 x,ev_int32 y)
				{
					if (!mDoubleClicked&& button!= mSelectButton&&!(mDraging&&mPickingMovable))
						||(mDoubleClicked&&button !=CGUIEvent::LEFT_MOUSE_BUTTON)
						return false;
					if (x==mDownX && y==mDownY&& isEditing()==false)
					{
						 mDraging =false;
						 mPickingMovable =NULL;
					}
					ev_bool ret= false;
					if (mEditable&& isEditing()&&mDraging)
					{
						if (x!=mDownX||y!=mDownY||!mDoubleClicked)
						{
							CViewport* vp = mControl->getViewport();
							CRay ray = ((CGlobeCamera*)mControl->getCamera())->screenToScreen(x,y,vp->getActualTop(),
								vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
							CEditBoundingBox* box = mPickingMovable->movable->getEditBoundingBox();
							if (box)
							{
								if (mTrackingType==CEditBoundingBox::TT_OnAxisX||mTrackingType==CEditBoundingBox::TT_OnAxisY
									||mTrackingType==CEditBoundingBox::TT_OnAxisZ)
								{
									if (mDownTime.msecsTo(CTime::currentTime())<200&&CMath::Abs(mDownX-x)<2
										&&CMath::Abs(mDownY-y)<2)
									{
										//点击坐标轴，进入旋转模式
										if (mTrackingType==CEditBoundingBox::TT_OnAxisX)
											box->showCircle(CEditBoundingBox::AT_X);
										else if(mTrackingType==CEditBoundingBox::TT_OnAxisY)
											box->showCircle(CEditBoundingBox::AT_Y);
										else if(mTrackingType==CEditBoundingBox::TT_OnAxisZ)
											box->showCircle(CEditBoundingBox::AT_Z);
									}
									else
									{
										if (mDownTime.msecsTo(CTime::currentTime())>200)
										{
											updateSelectdObjectTo(shift,x,y,IGlobeQueryListener::MOT_MouseUp);
										}
									}
								}
								else
								{
									if (mDownTime.msecsTo(CTime::currentTime())>200)
									{
										updateSelectdObjectTo(shift,x,y,IGlobeQueryListener::MOT_MouseUp);
									}
								}
							}
							ret = true;
						}
					}
					else if (!mDoubleClicked)
					{
						if ((x-mDownX)* (x-mDownX)+(y-mDownY)*(y-mDownY)>16)
						{
							return false;//鼠标按下和松开的位置相距较远，认为是在拖动地球
						}
						CViewport* vp = mControl->getViewport();
						CRay ray = ((CGlobeCamera*)mControl->getCamera())->screentoScreen(x,y,vp->getActualTop(),
							vp->getActualLeft(),vp->getActualWidth(),vp->getActualHeight());
						mSelector.setRay(ray);
						mSelector.setRenderSelected(true);
						clearLayerSelection();
						mSelector.setViewport(vp);
						EV_LOCK_AUTO_MUTEX;
						//先移除监听
						RaySceneQueryResult& lastresult = mSelector.getLastResults();
						for (RaySceneQueryResult::iterator iter= lastresult.begin();
							iter!=lastresult.end();++iter)
						{
							RaySceneQueryResultEntry& entry=*iter;
							entry.movable->removeListener(mpObjectListener);
						}
						//重新执行射线查询
						RaySceneQueryResult& result = mSelector.execute();
						//重新执行点查询
						CGlobeSelection selection;
						if (!mWgsSR)
						{
							mWgsSR = CCoordinateSystemFactory::createCoordSys(GEO_WGS84);
						}
						ev_real64 lat,lat1,lat2,lat3,lat4,lon,lon1,lon2,lon3,lon4,alt,alt1,alt2,alt3,alt4;
						ev_bool falg = false;
						ev_uint32 tol=2;
						ev_bool b = mControl->pickingRayIntersection(x,y,false,false,true,
							lat,lon,alt,flag,falg,falg);
						ev_bool b1=b && mControl->pickingRayIntersection(x-tol,y-tol,false,false,true,
							lat1,lon1,alt1,flag,falg,falg);
						ev_bool b2=b && b1 && mControl->pickingRayIntersection(x+tol,y-tol,false,false,true,
							lat2,lon2,alt2,flag,falg,falg);
						ev_bool b3=b &&b1&&b2&& mControl->pickingRayIntersection(x+tol,y+tol,false,false,true,
							lat3,lon3,alt3,flag,falg,falg);
						ev_bool b4=b &&b1&&b2&&b3&& mControl->pickingRayIntersection(x-tol,y+tol,false,false,true,
							lat4,lon4,alt4,flag,falg,falg);
						falg = false;
						if (b&&b1&&b2&&b3&&b4)
						{
							CPoint pt1(lon1,lat1);
							CPoint pt2(lon2,lat2);
							CPoint pt3(lon3,lat3);
							CPoint pt4(lon4,lat4);
							CLineString ls;
							ls.add(pt1,0);
							ls.add(pt2,1);
							ls.add(pt3,2);
							ls.add(pt4,3);
							ls.update();
							ls.setSpatialReferenceRef(mWgsSR);
							CCurveRing ring;
							ring.add(ls,0);
							ring.update();
							ring.setSpatialReferenceRef(mWgsSR);
							CPolygon polygon;
							polygon.addExteriorRing(ring);
							polygon.update();
							polygon.setSpatialReferenceRef(mWgsSR);
							CQueryFilter qf;
							qf.setFieldName("*");
							qf.setQueryMode(EVQueryModeSpatial);
							qf.setQueryGeometry(&polygon);
							qf.setSpatialRelation(EVSQRelTypeIntersects);
							CGeoSceneManager* sm = (CGeoSceneManager*)(mControl->getSceneManager());
							if (queryClampFeatureLayer(sm->getFeatureGroupLayer(),&qf,selection))
							{
								falg = true;
							}
						}
						//执行选择结果
					    for (RaySceneQueryResult::iterator iter = result.begin();
							iter!=result.end();++iter)
					    {
							RaySceneQueryResultEntry& entry =*iter;
							entry.movable->addListener(mpObjectListener);
					    }
						mControl->fireSelectionChanged(result,selection);
						mPickingMovable = NULL;
						if(mSelectButton==CGUIEvent::LEFT_MOUSE_BUTTON)
							ret = true;
					}
					mDraging = false;
					mDoubleClicked = false;
					return ret;
				}
				ev_bool CRaySelectTool::queryClampFeatureLayer(CFeatureGroupLayer* gl,IQueryFilter* qf,CGlobeSelection& selection)
				{
					ev_bool query =false;
					for (ev_uint32 index =0;index<gl->getlayerCount();++index)
					{
						if (IClampSceneLayer* layer = dynamic_cast<IClampSceneLayer*>(gl->getLayer(index)))
						{
							if (layer->isVisible()&&layer->isSelectable())
							{
								layer->select(qf,SRT_New);
								ILayerSelection* pLayerSelection = layer->getLayerSelection();
								if (pLayerSelection!=NULL)
								{
									IFeatureSelection* fs = pLayerSelection->getSelection();
									if (fs!=NULL&&fs->getCount())
									{
										selection.add(layer,pLayerSelection);
										query = true;
									}
								}
							}
						}
						else if (CFeatureGroupLayer* layer2 = dynamic_cast<CFeatureGroupLayer*>(gl->getLayer(index)))
						{
							bool flag = queryClampFeatureLayer(layer2,qf,selection);
							if (flag == true)
							{
								query = true;
							}
						}
					}
					return query;
				}
				ev_bool CRaySelectTool::onKeyDown(ev_int32 keyCode,ev_int32 shift)
				{
					if (mEditable && keyCode == CGUIEvent::KEY_Delete)
					{
						mControl->fireDeleteKeyPressed();
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
}