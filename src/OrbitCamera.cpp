#include "OrbitCamera.h"
#include <QQuaternion>

const float MinYaw = -89.99f;
const float MaxYaw = 89.99f;

OrbitCamera::OrbitCamera(float aspectRatio, float fieldOfView, QVector3D lookAt, QVector3D up, float nearPlane, float farPlane)
{
	this->aspectRatio = aspectRatio;
	this->fieldOfView = fieldOfView;
	this->lookAt = lookAt;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	viewMatrixDirty = true;
	projectionMatrixDirty = true;
	pitch = 0;
	yaw = 0;
	zoom = 2;
}

void OrbitCamera::MoveCameraRight(float amount)
{
	QVector3D right = lookAt - position; //calculate forward
	right.normalize();
	right = QVector3D::crossProduct(right, QVector3D(0, 1, 0)); //calculate the real right
	right.setY(0);
	right.normalize();
	lookAt += right * amount;
	viewMatrixDirty = true;
}

void OrbitCamera::MoveCameraForward(float amount)
{
	QVector3D forward = lookAt - position;
	forward.normalize();
	forward.setY(0);
	forward.normalize();
	lookAt += forward * amount;
	viewMatrixDirty = true;
}

void OrbitCamera::ReCreateViewMatrix()
{
	//Calculate the relative position of the camera
	QMatrix4x4 rot;
	rot.rotate(QQuaternion::fromEulerAngles(yaw, pitch, 0));
	position = rot * QVector3D(0, 0, 1);
	position *= zoom;
	position += lookAt;

	//Calculate a new viewmatrix
	m_ViewMatrix.setToIdentity();
	m_ViewMatrix.lookAt(position, lookAt, QVector3D(0,1,0));
	viewMatrixDirty = false;
}

void OrbitCamera::ReCreateProjectionMatrix()
{
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	projectionMatrixDirty = false;
}


QVector3D OrbitCamera::Right()
{
	QVector3D right = Forward();
	right = QVector3D::crossProduct(right, QVector3D(0, 1, 0));
	right.normalize();
	return right;
}

QVector3D OrbitCamera::Forward()
{
	QVector3D forward = lookAt - position; //calculate forward
	forward.normalize();
	return forward;
}

QVector3D OrbitCamera::Up()
{
	QVector3D up = Forward();
	up = QVector3D::crossProduct(up, Right());
	up.normalize();
	return up;
}

QMatrix4x4 OrbitCamera::ViewProjectionmatrix()
{
	if (viewMatrixDirty) ReCreateViewMatrix();
	if (projectionMatrixDirty) ReCreateProjectionMatrix();

	return projectionMatrix * m_ViewMatrix;
}

void OrbitCamera::SetYaw(float value)
{
	viewMatrixDirty = true; 
	yaw = qBound(MinYaw, value, MaxYaw);
}

