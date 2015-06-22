
#include "GLImageWidget.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QTimer>

GLImageWidget::GLImageWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	renderTexture(this),
	rect(this),
	currentVertexIndex(0)
{
	// loop call update
	 QTimer *timer = new QTimer(this);
	 connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	 timer->start(33);
}


GLImageWidget::~GLImageWidget()
{
	cleanup();
}


void GLImageWidget::setImage(const QImage& inputImage)
{
	makeCurrent();
	renderTexture.setImage(inputImage);
	update();
}



void GLImageWidget::setImage(const QString& fileImage)
{
	setImage(QImage(fileImage));
}


void GLImageWidget::cleanup()
{
	this->makeCurrent();
	renderTexture.cleanup();
	rect.cleanup();
	this->doneCurrent();
}


void GLImageWidget::initializeGL()
{
	this->initializeOpenGLFunctions();

#if 0
	// get context opengl-version
	qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
	qDebug() << "Context valid: " << context()->isValid();
	qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
	qDebug() << "OpenGL information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
	qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
	qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
	qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
#endif

	renderTexture.initialize();
	rect.initialize();
}




void GLImageWidget::paintGL()
{
	//renderTexture.render();
	rect.render();
}


void GLImageWidget::resizeGL(int w, int h)
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glViewport(0, 0, this->width(), this->height());
}


void GLImageWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MouseButton::MiddleButton)
		clickBegin = event->pos();
}


void GLImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MouseButton::MiddleButton)
	{
		clickEnd = event->pos();
	}

	if (event->button() == Qt::MouseButton::RightButton)
	{
	}

	if (event->button() == Qt::MouseButton::LeftButton)
	{
		if (Qt::ControlModifier == QApplication::keyboardModifiers())
		{
			float x = float(event->x()) / float(width());
			float y = 1.0f - float(event->y()) / float(height());

			if (currentVertexIndex < 3)
				rect.setVertex(currentVertexIndex++, QVector3D(-0.8f, -0.8f, 0.0f));

			std::cout << currentVertexIndex << " : " << x << ", " << y << std::endl;
		}
	}
	
	
	this->update();
}


void GLImageWidget::mouseMoveEvent(QMouseEvent *event)
{
}


void GLImageWidget::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	//case Qt::Key_Q:
	//	close();
	//	break;
	case Qt::Key_Control:
		currentVertexIndex = 0;
		break;

	default:
		QWidget::keyReleaseEvent(event);
	}

	update();
}


void GLImageWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		//case Qt::Key_Q:
		//	close();
		//	break;
	case Qt::Key_Control:
		currentVertexIndex = 0;
		break;

	default:
		QWidget::keyPressEvent(event);
	}

	update();
}

