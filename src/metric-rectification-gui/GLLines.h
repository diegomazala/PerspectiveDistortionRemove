#ifndef _GL_LINES_H_
#define _GL_LINES_H_

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QVector3D>
#include <QVector>



class GLLines : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLLines(int line_count, QObject *parent = 0);
	virtual ~GLLines();

public slots:
	virtual void initialize();
	virtual void render();
	virtual void cleanup();
	void onEnable(bool enable);
	void reset();

	void setVertexLine(int line_index, int vertex_index, QVector3D v);
	void setVertices(int vertex_index, QVector3D v);
	void computeCanonicalVertices(int width, int height);
	
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
	bool enabled;
	const int maxLines;
};


#endif // _GL_LINES_H_
