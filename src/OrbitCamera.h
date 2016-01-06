/*
	Modelizer - Model file converter
	Copyright (C) 2015 Frank Köhnke

	This file is part of Modelizer.

	Modelizer is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License 
	as published by the Free Software Foundation; either 
	version 3 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 *	@file OrbitCamera.h
 */
#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class OrbitCamera
{
public:
	OrbitCamera(float aspectRation, QVector3D lookAt)
		: OrbitCamera(aspectRation, 45.0f, lookAt, QVector3D(0.0f,1.0f,0.0f), 0.1f, 1000.f) { }


	OrbitCamera(float aspectRatio, float fieldOfView, QVector3D lookAt, QVector3D up, float nearPlane, float farPlane);

	void MoveCameraRight(float amount);
	void MoveCameraForward(float amount);

	float Pitch() { return pitch; }
	void SetPitch(float value) { viewMatrixDirty = true; pitch = value; }

	float Yaw() { return yaw; }
	void SetYaw(float value);

	float FieldOfView() const { return fieldOfView; }
	void SetFieldOfView(float val) { projectionMatrixDirty = true;  fieldOfView = val; }

	float AspectRatio() const { return aspectRatio; }
	void SetAspectRatio(float val) { projectionMatrixDirty = true; aspectRatio = val; }

	float NearPlane() const { return nearPlane; }
	void NearPlane(float val) { projectionMatrixDirty = true; nearPlane = val; }

	float FarPlane() const { return farPlane; }
	void FarPlane(float val) { projectionMatrixDirty = true; farPlane = val; }

	float Zoom() const { return zoom; }
	void SetZoom(float val) { viewMatrixDirty = true; zoom = qMax(0.01f, val); }

	QVector3D Position() { if (viewMatrixDirty) ReCreateViewMatrix(); return position; }

	QVector3D LookAt() const { return lookAt; }
	void SetLookAt(QVector3D val) { viewMatrixDirty = true; lookAt = val; }

	QMatrix4x4 ViewProjectionmatrix();

	QMatrix4x4 ViewMatrix() { if (viewMatrixDirty) ReCreateViewMatrix(); return m_ViewMatrix; }

	QMatrix4x4 ProjectionMatrix() { if (projectionMatrixDirty) ReCreateProjectionMatrix(); return projectionMatrix; }

	QVector3D Right();
	QVector3D Forward();
	QVector3D Up();
private:
	void ReCreateViewMatrix();
	void ReCreateProjectionMatrix();

	float pitch;
	float yaw;
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float zoom;
	
	QVector3D position;	
	QVector3D lookAt;

	QMatrix4x4 m_ViewMatrix;
	QMatrix4x4 projectionMatrix;
	
	bool viewMatrixDirty;
	bool projectionMatrixDirty;

};