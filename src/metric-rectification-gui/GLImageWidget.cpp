
#include "GLImageWidget.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QFile>
#include <QTimer>

GLImageWidget::GLImageWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	renderTexture(this),
	lines(10, this),
	currentLineIndex(0),
	currentVertexIndex(0),
	maxLines(10)
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

void GLImageWidget::setCurrentLine(int index)
{
	currentVertexIndex = 0;
	currentLineIndex = index;
}


void GLImageWidget::setLinesEnable(bool enabled)
{
	lines.onEnable(enabled);
}

void GLImageWidget::setImage(const QImage& inputImage)
{
	makeCurrent();
	renderTexture.setImage(inputImage);
	lines.reset();
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
	lines.cleanup();
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
	lines.initialize();
		
	glLineWidth(5);
}




void GLImageWidget::paintGL()
{
	renderTexture.render();
	lines.render();
}


void GLImageWidget::resizeGL(int w, int h)
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glViewport(0, 0, this->width(), this->height());
}


void GLImageWidget::mousePressEvent(QMouseEvent *event)
{
	if (currentLineIndex > -1 && currentLineIndex < maxLines && 
		currentVertexIndex > -1 && currentVertexIndex < 2)
	{
		int img_x = float(event->x()) / float(width()) * float(renderTexture.width());
		int img_y = float(event->y()) / float(height()) * float(renderTexture.height());

		emit newPoint(currentLineIndex, currentVertexIndex, img_x, img_y);		

		// normalizing values [(-1,-1):(1,1)]
		float x = float(event->x()) / float(width()) * 2.0f - 1.0f;
		float y = (float(event->y()) / float(height()) * 2.0f - 1.0f) * (-1.0f);	// inverting y coordinate
		float z = 0.f;

		lines.setVertexLine(currentLineIndex, currentVertexIndex, QVector3D(x, y, z));
		++currentVertexIndex;

		if (currentVertexIndex == 2)					// go to second line of the pair
		{	
			currentVertexIndex = 0;
			currentLineIndex++;
		}

	}

}


void GLImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
	if (event->button() == Qt::MouseButton::MiddleButton)
	{
		clickEnd = event->pos();
	}

	if (event->button() == Qt::MouseButton::RightButton)
	{
	}

	if (event->button() == Qt::MouseButton::LeftButton)
	{
		if (currentLineIndex > -1 && currentLineIndex < maxLines)
		{
			int img_x = float(event->x()) / float(width()) * float(renderTexture.width());
			int img_y = float(event->y()) / float(height()) * float(renderTexture.height());

			emit newPoint(currentLineIndex, 0, img_x, img_y);

			// normalizing values [(-1,-1):(1,1)]
			float x = float(event->x()) / float(width()) * 2.0f - 1.0f;
			float y = (float(event->y()) / float(height()) * 2.0f - 1.0f) * (-1.0f);	// inverting y coordinate
			float z = 0.f;

			lines.setVertexLine(currentLineIndex, 1, QVector3D(x, y, z));
		}
	}
#endif
	
	this->update();
}


void GLImageWidget::mouseMoveEvent(QMouseEvent *event)
{

	if (currentLineIndex > -1 && currentLineIndex < maxLines &&
		currentVertexIndex > -1 && currentVertexIndex < 2)
	{
		int ximg = float(event->x()) / float(width()) * float(renderTexture.width());
		int yimg = float(event->y()) / float(height()) * float(renderTexture.height());
		emit mouseMove(ximg, yimg);

		// normalizing values [(-1,-1):(1,1)]
		float x = float(event->x()) / float(width()) * 2.0f - 1.0f;
		float y = (float(event->y()) / float(height()) * 2.0f - 1.0f) * (-1.0f);	// inverting y coordinate
		float z = 0.f;

		lines.setVertexLine(currentLineIndex, currentVertexIndex, QVector3D(x, y, z));
	}

	update();
}


void GLImageWidget::keyReleaseEvent(QKeyEvent *event)
{
}


void GLImageWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		
	case Qt::Key_Escape:
		currentVertexIndex = 0;
		break;

	default:
		QWidget::keyPressEvent(event);
	}

	update();
}

