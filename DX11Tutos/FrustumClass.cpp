#include "FrustumClass.h"



FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass &)
{
}


FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMinimum, r;
	XMFLOAT4X4 matrix;
	XMFLOAT4X4 projectionMatrix4x4, viewMatrix4x4;
	XMVECTOR tempPlane;

	XMStoreFloat4x4(&projectionMatrix4x4, projectionMatrix);
	XMStoreFloat4x4(&viewMatrix4x4, viewMatrix);

	zMinimum = -projectionMatrix4x4._43 / projectionMatrix4x4._33;

	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix4x4._33 = r;
	projectionMatrix4x4._43 = -r * zMinimum;

	XMMATRIX tempMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	XMStoreFloat4x4(&matrix, tempMatrix);

	// Calculate near plane of frustum.
	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;
	tempPlane = XMLoadFloat4(&m_planes[0]);
	tempPlane = XMPlaneNormalize(tempPlane);
	XMStoreFloat4(&m_planes[0], tempPlane);

	// Calculate far plane of frustum.
	m_planes[1].x = matrix._14 - matrix._13;
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	tempPlane = XMLoadFloat4(&m_planes[1]);
	tempPlane = XMPlaneNormalize(tempPlane);
	XMStoreFloat4(&m_planes[1], tempPlane);

	// Calculate left plane of frustum.
	m_planes[2].x = matrix._14 + matrix._11;
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	tempPlane = XMLoadFloat4(&m_planes[2]);
	tempPlane = XMPlaneNormalize(tempPlane);
	XMStoreFloat4(&m_planes[2], tempPlane);

	// Calculate right plane of frustum.
	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	tempPlane = XMLoadFloat4(&m_planes[3]);
	tempPlane = XMPlaneNormalize(tempPlane);
	XMStoreFloat4(&m_planes[3], tempPlane);

	// Calculate top plane of frustum.
	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	tempPlane = XMLoadFloat4(&m_planes[4]);
	tempPlane = XMPlaneNormalize(tempPlane);
	XMStoreFloat4(&m_planes[4], tempPlane);

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	tempPlane = XMLoadFloat4(&m_planes[5]);
	tempPlane = XMPlaneNormalize(tempPlane);
	XMStoreFloat4(&m_planes[5], tempPlane);
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;


	// Check if the point is inside all six planes of the view frustum.
	for (i = 0; i < 6; i++)
	{
		XMVECTOR tempPlane = XMLoadFloat4(&m_planes[i]);
		XMVECTOR tempVector = XMLoadFloat3(&XMFLOAT3(x, y, z));
		XMFLOAT4 tempRes;

		XMStoreFloat4(&tempRes, XMPlaneDotCoord(tempPlane, tempVector));
		if (tempRes.x < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool FrustumClass::CheckCube(float, float, float, float)
{
	return false;
}

bool FrustumClass::CheckSphere(float, float, float, float)
{
	return false;
}

bool FrustumClass::CheckRectangle(float, float, float, float, float, float)
{
	return false;
}
