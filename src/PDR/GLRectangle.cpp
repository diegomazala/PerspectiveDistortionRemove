
#include "GLRectangle.h"
#include <iostream>


GLRectangle::GLRectangle(QObject *parent)
	: QObject(parent),
	drawMode(GL_POINTS)
{
	vertices << QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, 1.0f, 0.0f)
		<< QVector3D(1.0f, 1.0f, 0.0f)
		<< QVector3D(1.0f, -1.0f, 0.0f);
}


GLRectangle::~GLRectangle()
{
}





void GLRectangle::cleanup()
{
	vbo->destroy();
}


void GLRectangle::initialize()
{
	initializeOpenGLFunctions();
	
	program.reset(buildProgram());
	program->release();

	vao.create();
	vao.bind();
	vbo.reset(buildVertexBuffer());
	vao.release();
}


void GLRectangle::setDrawMode(GLenum draw_mode)
{
	drawMode = draw_mode;
}


void GLRectangle::setVertex(int index, QVector3D v)
{
	vertices[index] = v;
	//updateVertexBuffer();

	vbo->bind();
	vbo->write(index * sizeof(QVector3D), &v[0], sizeof(QVector3D));
	vbo->release();
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
	program->bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glDrawArrays(drawMode, 0, vertices.size());
	program->release();
	vao.release();
}



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



QOpenGLBuffer* GLRectangle::buildVertexBuffer()
{
	QOpenGLBuffer* lpvbo = new QOpenGLBuffer();

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

