#ifndef _GL_RENDER_TEXTURE_H_
#define _GL_RENDER_TEXTURE_H_

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QImage>



class GLRenderTexture : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLRenderTexture(QObject *parent = 0);
	virtual ~GLRenderTexture();

	QImage result() const { return renderFbo->toImage(); }

public slots:
	virtual void initialize();
	virtual void render();
	virtual void run();
	virtual void plot(int w, int h);
	virtual void cleanup();
	virtual void setImage(const QImage& image);
	

protected:	// methods

	virtual void buildTexture();
	virtual QOpenGLShaderProgram* buildProgram() const;
	virtual QOpenGLShaderProgram* buildPassthorughProgram() const;
	virtual QOpenGLBuffer* buildQuadTextured() const;
	
	

protected : // attributes
	QOpenGLVertexArrayObject vao;
	QScopedPointer<QOpenGLBuffer> vbo;
	
	QSharedPointer<QOpenGLTexture> texture;
	QScopedPointer<QOpenGLShaderProgram> program;
	QScopedPointer<QOpenGLShaderProgram> plotProgram;
	QSharedPointer<QOpenGLFramebufferObject> renderFbo;

	QImage image;

};


#endif // _GL_RENDER_TEXTURE_H_
