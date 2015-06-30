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
	void onEnable(bool enable);
	void reset();

	void setVertex(int index, QVector3D v);
	void setVertices(const QVector<QVector3D>& verts);
	void setDrawMode(GLenum draw_mode);
	
protected:	// methods

	virtual QOpenGLShaderProgram* buildProgram() const;
	virtual QOpenGLBuffer* buildVertexBuffer();
	virtual void updateVertexBuffer();


protected: // attributes
	QOpenGLVertexArrayObject vao;
	QScopedPointer<QOpenGLBuffer> vbo;
	QScopedPointer<QOpenGLShaderProgram> program;
	QVector<QVector3D> vertices;
	bool drawVertices;
	GLenum drawMode;
	bool enabled;
};


#endif // _GL_RECTANGLE_H_
