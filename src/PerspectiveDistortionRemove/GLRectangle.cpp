
#include "GLRectangle.h"
#include <iostream>


GLRectangle::GLRectangle(QObject *parent)
	: QObject(parent),
	image(1, 1, QImage::Format::Format_ARGB32)
{
	image.fill(0);
}


GLRectangle::~GLRectangle()
{
}





void GLRectangle::cleanup()
{
	vbo->destroy();
	texture.clear();
	renderFbo.clear();
}


void GLRectangle::initialize()
{
	initializeOpenGLFunctions();

	texture.reset(new QOpenGLTexture(image.mirrored()));
	texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);

	program.reset(buildProgram());
	program->release();

	vao.create();
	vao.bind();
	vbo.reset(buildQuadTextured());
	vao.release();

	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	renderFbo.reset(new QOpenGLFramebufferObject(image.size(), format));
}

void GLRectangle::setVertex(int index, QVector3D v)
{
	vertices[index] = v;
	updateVertexBuffer();
}

void GLRectangle::setVertices(const QVector<QVector3D>& verts)
{
	vertices = verts;
	updateVertexBuffer();
}

void GLRectangle::updateVertexBuffer()
{
	vbo->bind();
	vbo->write(0, vertices.constData(), vertices.size() * sizeof(QVector3D));
	vbo->release();
}


void GLRectangle::render()
{
	vao.bind();

	//vertices.clear();
	//vertices << QVector3D(-0.8f, -0.8f, 0.0f)
	//	<< QVector3D(-0.8f, 0.8f, 0.0f)
	//	<< QVector3D(0.8f, 0.8f, 0.0f)
	//	<< QVector3D(0.8f, -0.8f, 0.0f);
	//updateVertexBuffer();

	if (!texture.isNull())
		texture->bind();

	program->bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glDrawArrays(GL_QUADS, 0, 4);
	program->release();

	vao.release();
}



// Build a pass through glsl program
QOpenGLShaderProgram* GLRectangle::buildProgram() const
{
	QOpenGLShaderProgram* prog = new QOpenGLShaderProgram();

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, prog);
	const char *vsrc =
		"attribute highp vec4 in_Vertex;\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = in_Vertex;\n"
		"}\n";
	vshader->compileSourceCode(vsrc);


	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, prog);
	const char *fsrc =
		"void main(void)\n"
		"{\n"
		"    gl_FragColor = vec4(1.0f);\n"
		"}\n";
	fshader->compileSourceCode(fsrc);

	prog->addShader(vshader);
	prog->addShader(fshader);
	prog->bindAttributeLocation("in_Vertex", 0);
	prog->link();
	prog->bind();
	prog->release();

	return prog;
}



QOpenGLBuffer* GLRectangle::buildQuadTextured()
{
	QOpenGLBuffer* lpvbo = new QOpenGLBuffer();

	vertices << QVector3D(-0.5f, -0.5f, 0.0f)
		<< QVector3D(-0.5f, 0.5f, 0.0f)
		<< QVector3D(0.5f, 0.5f, 0.0f)
		<< QVector3D(0.5f, -0.5f, 0.0f);

	const size_t BufferSize = vertices.size() * sizeof(QVector3D);
	const size_t VertexSize = sizeof(QVector3D);

	// Setup our vertex buffer object.
	lpvbo->create();
	lpvbo->bind();
	lpvbo->allocate(vertices.data(), (int)BufferSize);
	lpvbo->setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);

	lpvbo->bind();

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, 0);

	lpvbo->release();

	return lpvbo;
}

