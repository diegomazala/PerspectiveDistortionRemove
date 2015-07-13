#ifndef __GL_CROSS_RATIO_LINES_H__
#define __GL_CROSS_RATIO_LINES_H__

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



class GLCrossRatioLines : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLCrossRatioLines(QObject *parent = 0);
	virtual ~GLCrossRatioLines();

public slots:
	virtual void initialize();
	virtual void render();
	virtual void cleanup();
	void onEnable(bool enable);
	void reset();

	void setVertexLine(int line_index, int vertex_index, QVector3D v);
	
protected:	// methods

	virtual QOpenGLShaderProgram* buildLineProgram() const;
	virtual QOpenGLShaderProgram* buildPointProgram() const;
	virtual QOpenGLBuffer* buildVertexBuffer();
	virtual void updateVertexBuffer();


protected: // attributes
	QOpenGLVertexArrayObject vao;
	QScopedPointer<QOpenGLBuffer> vbo;
	QScopedPointer<QOpenGLShaderProgram> lineProgram;
	QScopedPointer<QOpenGLShaderProgram> pointProgram;
	QVector<QVector3D> vertices;
	bool drawVertices;
	bool enabled;
};


#endif // __GL_CROSS_RATIO_LINES_H__
