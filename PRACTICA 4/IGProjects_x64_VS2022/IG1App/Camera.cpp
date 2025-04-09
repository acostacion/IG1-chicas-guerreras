#include "Shader.h"
#include "Camera.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IG1App.h"

using namespace glm;

Camera::Camera(Viewport* vp)
	: mViewMat(1.0)
	, mProjMat(1.0)
	, xRight(vp->width() / 2.0)
	, xLeft(-xRight)
	, yTop(vp->height() / 2.0)
	, yBot(-yTop)
	, mViewPort(vp)
{
	setPM();
}

void Camera::setAxes()
{
	// para los ejes right=u, upward=v, front=-n
	mRight = row(mViewMat, 0);
	mUpward = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}

void Camera::moveLR(GLfloat cs) // LEFT / RIGHT.
{
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void Camera::moveFB(GLfloat cs) // FRONT / BACK.
{
	mEye += mFront * cs;
	mLook += mFront * cs;
	setVM();
}

void Camera::moveUD(GLfloat cs) // UP / DOWN.
{
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}

void Camera::changePrj()
{
	bOrto = !bOrto;
	setPM();
}

void Camera::pitchReal(GLfloat cs) //Rotacion en x (u)
{
	//Dice si
	mProjMat = rotate(mProjMat, (double)(glm::radians(cs)), glm::dvec3(1.0, 0, 0));
	setVM();
}

void Camera::yawReal(GLfloat cs) //Rotacion en y (v)
{
	//Dice no
	mProjMat = rotate(mProjMat, (double)(glm::radians(cs)), glm::dvec3(0, 1.0, 0));
	setVM();
}
void Camera::rollReal(GLfloat cs) //Rotacion en z (n)
{
	mProjMat = rotate(mProjMat, (double)(glm::radians(cs)), glm::dvec3(0, 0, 1.0));
	setVM();
}

void Camera::orbit(GLdouble incAng)
{
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	setVM();
}

void Camera::orbit(GLdouble incAng, GLdouble incY)
{
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	setVM();
}

void
Camera::uploadVM() const
{
	/*dmat4 ae = (-1.0f, -1.0f, -1.0f);
	mViewMat = mViewMat * ae;*/

	Shader::setUniform4All("lightDir", mViewMat);
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = { 0, 0, 500 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 1, 0 };
	mAng = 0;
	mRadio = 500;

	setVM();
}

void
Camera::set3D()
{
	mEye = { 500, 500, 500 };
	mLook = { 0, 10, 0 };
	mUp = { 0, 1, 0 };

	mAng = 0;
	mRadio = 500;

	setVM();
}

void Camera::setCenital()
{
	mEye = { 0, 500, 0 };
	mLook = { 0, 0, 0 };
	mUp = { 1, 0, 0 };

	mAng = 0;
	mRadio = 500;

	setVM();
}

void
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
}

glm::dvec3 Camera::row(glm::dmat4 matrix, int index)
{
	return matrix[index];
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			mNearVal,
			mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else
	{
		//Proyeccion perspectica
		mProjMat = frustum(xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			mNearVal * 200, // el problema que hay es que se ve de muy cerca. (500)
			mFarVal);
	}
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}