#ifndef _GL_RECTANGLE_H_
#define _GL_RECTANGLE_H_

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QVector3D>
#include <QVector>



class GLRectangle : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLRectangle(QObject *parent = 0);
	virtual ~GLRectangle();

	public slots:
	virtual void initialize();
	virtual void render();
	virtual void cleanup();

	void setVertex(int index, QVector3D v);
	void setVertices(const QVector<QVector3D>& verts);
	
protected:	// methods

	virtual QOpenGLShaderProgram* buildProgram() const;
	virtual QOpenGLBuffer* buildQuadTextured();
	virtual void updateVertexBuffer();


protected: // attributes
	QOpenGLVertexArrayObject vao;
	QScopedPointer<QOpenGLBuffer> vbo;

	QSharedPointer<QOpenGLTexture> texture;
	QScopedPointer<QOpenGLShaderProgram> program;
	QScopedPointer<QOpenGLShaderProgram> plotProgram;
	QSharedPointer<QOpenGLFramebufferObject> renderFbo;

	QImage image;

	QVector<QVector3D> vertices;
};


#endif // _GL_RECTANGLE_H_
