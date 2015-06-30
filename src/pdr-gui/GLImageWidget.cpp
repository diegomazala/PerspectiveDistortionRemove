
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
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	setFocus();
	//setFlag(Qt::WA_Hover);
	setMouseTracking(true);

	// loop call update
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(33);
}


GLImageWidget::~GLImageWidget()
{
	cleanup();
}


void GLImageWidget::setRectEnable(bool enabled)
{
	rect.onEnable(enabled);
}

void GLImageWidget::setImage(const QImage& inputImage)
{
	makeCurrent();
	renderTexture.setImage(inputImage);
	rect.reset();
	update();

	setFocus();
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
	renderTexture.render();
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
		if (Qt::ControlModifier == QApplication::keyboardModifiers() || Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			if (currentVertexIndex < 4)
			{
				int img_x = float(event->x()) / float(width()) * float(renderTexture.width());
				int img_y = float(event->y()) / float(height()) * float(renderTexture.height());

				emit newPoint(currentVertexIndex, img_x, img_y);

				// normalizing values [(-1,-1):(1,1)]
				float x = float(event->x()) / float(width()) * 2.0f - 1.0f;
				float y = (float(event->y()) / float(height()) * 2.0f - 1.0f) * (-1.0f);	// inverting y coordinate
				float z = 0.f;

				rect.setVertex(currentVertexIndex++, QVector3D(x, y, z));
			}
		}
	}
	
	
	this->update();
}


void GLImageWidget::mouseMoveEvent(QMouseEvent *event)
{
	int ximg = float(event->x()) / float(width()) * float(renderTexture.width());
	int yimg = float(event->y()) / float(height()) * float(renderTexture.height());
	emit mouseMove(ximg, yimg);

	// normalizing values [(-1,-1):(1,1)]
	float x = float(event->x()) / float(width()) * 2.0f - 1.0f;
	float y = (float(event->y()) / float(height()) * 2.0f - 1.0f) * (-1.0f);	// inverting y coordinate
	float z = 0.f;

	if (currentVertexIndex < 4)
		rect.setVertex(currentVertexIndex, QVector3D(x, y, z));

	update();
}


void GLImageWidget::keyReleaseEvent(QKeyEvent *event)
{
	//switch (event->key())
	//{
	//case Qt::Key_Control:
	//	rect.setDrawMode(GL_LINE_LOOP);
	//	currentVertexIndex = 0;
	//	break;

	//default:
	//	QWidget::keyReleaseEvent(event);
	//}

	//update();
}


void GLImageWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		
	case Qt::Key_Control:
		currentVertexIndex = 0;
		rect.setDrawMode(GL_LINE_LOOP);
		break;

	case Qt::Key_Shift:
		currentVertexIndex = 0;
		rect.setDrawMode(GL_POINTS);
		break;

	case Qt::Key_L:
		rect.setDrawMode(GL_LINE_LOOP);
		break;

	case Qt::Key_P:
		rect.setDrawMode(GL_POINTS);
		break;

	default:
		QWidget::keyPressEvent(event);
	}

	update();
}

