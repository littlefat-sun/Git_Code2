void CMatrix3::FromEulerAnglesXYZ(const CRadian& fYAngle,
	const CRadian& fPAngle,const CRadian& fRAngle)
{
	  Real fCos, fSin;
	  fCos = CMath::Cos(fYAngle);
	  fSin = CMath::Sin(fYAngle);
	  CMatrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);
	  fCos = CMath::Cos(fPAngle);
	  fSin = CMath::Sin(fPAngle);
	  CMatrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);
	  fCos = CMath::Cos(fRAngle);
	  fSin = CMath::Sin(fRAngle);
	  CMatrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos ,0.0, 0.0, 0.0,1.0);
	  *this = kXMat* (kYMat*kZMat);
}
void CMatrix3::FromEulerAnglesXZY(const CRadian& fYAngle,
	const CRadian& fPAngle,const CRadian& fRAngle)
{
	Real fCos, fSin;
	fCos = CMath::Cos(fYAngle);
	fSin = CMath::Sin(fYAngle);
	CMatrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);
	fCos = CMath::Cos(fPAngle);
	fSin = CMath::Sin(fPAngle);
	CMatrix3 kYMat(fCos,-fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);
	fCos = CMath::Cos(fRAngle);
	fSin = CMath::Sin(fRAngle);
	CMatrix3 kZMat(fCos, 0.0, fSin, 0.0, 1.0 ,0.0, -fSin, 0.0,fCos);
	*this = kXMat* (kYMat*kZMat);
}
{
	CVector3 planPos;
	CMatrix4 worldMatrix;//世界矩阵
	planPos = CMathUtility::sphericalToCartesian(10.11,110.37,CMath::EARTH_RADIUS+100);//世界坐标
	CVector3 yAxis(planPos.x,planPos.y,planPos.z);//正北平行地球矩阵
	CVector3 zAxis=CVector3::UNIT_Z;
	CVector3 xAxis= yAxis.crossProduct(zAxis);
	zAxis = xAxis.crossProduct(yAxis);
	zAxis.normalise();
	yAxis.normalise();
	xAxis.normalise();
	qMat[0][0]= xAxis.x;
	qMat[1][0]= xAxis.y;
	qMat[2][0]= xAxis.z;
	qMat[0][1]= yAxis.x;
	qMat[1][1]= yAxis.y;
	qMat[2][1]= yAxis.z;
	qMat[0][2]= zAxis.x;
	qMat[1][2]= zAxis.y;
	qMat[2][2]= zAxis.z;
	CMatrix3 ratMatrix=CMatrix3::IDENTITY;
	//俯仰、偏航、翻转
	ratMatrix.FromEulerAnglesXYZ(CMath::DegressToRadians(0.0),CRadian(CMath::DegreesToRadians(-90.0)),CRadian(CMath::DegreesToRadians(0.0)));
	CMatrix4 rot = ratMatrix;
	worldMatrix = qMat * rot;
	CMatrix4 scaleMatrix;
	scaleMatrix.setScale(CVector3(1.5));//缩放矩阵
	worldMatrix = worldMatrix * scaleMatrix;
	worldMatrix[0][3] = planPos.x;//模型位置世界坐标
	worldMatrix[1][3] = planPos.y;
	worldMatrix[2][3] = planPos.z;
	mpBattleplaneNode->setMatrix(worldMatrix);
}