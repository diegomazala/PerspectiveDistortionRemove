#ifndef _GL_IMAGE_WIDGET_H_
#define _GL_IMAGE_WIDGET_H_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QScopedPointer>
#include <QOpenGLShaderProgram>
#include <QImage>
#include <QPoint>
#include "GLRenderTexture.h"
#include "GLRectangle.h"

class QMouseEvent;
class QKeyEvent;

class GLImageWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:

	GLImageWidget(QWidget *parent = 0);
	~GLImageWidget();

	QSize minimumSizeHint() const Q_DECL_OVERRIDE{ return QSize(50, 50); };
	QSize sizeHint() const Q_DECL_OVERRIDE{ return QSize(1280, 720); };
	
public slots:
	virtual void cleanup();
	void setImage(const QImage& image);
	void setImage(const QString& fileimage);
	void setRectEnable(bool enabled);
	
signals:
	void mouseMove(int, int);
	void newPoint(int,int, int);

protected:	// methods

	virtual void initializeGL() Q_DECL_OVERRIDE;
	virtual void paintGL() Q_DECL_OVERRIDE;
	virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;

	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;


protected : // attributes
	
	GLRenderTexture renderTexture;
	GLRectangle		rect;
	QPoint clickBegin;
	QPoint clickEnd;
	int currentVertexIndex;
};


#endif // _GL_IMAGE_WIDGET_H_
