
#include "GLCrossRatioLines.h"
#include <iostream>


GLCrossRatioLines::GLCrossRatioLines(QObject *parent)
	: QObject(parent),
	enabled(true)
{
	vertices
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f);
}


GLCrossRatioLines::~GLCrossRatioLines()
{
}

void GLCrossRatioLines::onEnable(bool enable)
{
	enabled = enable;
}



void GLCrossRatioLines::cleanup()
{
	vbo->destroy();
}


void GLCrossRatioLines::initialize()
{
	initializeOpenGLFunctions();
	
	lineProgram.reset(buildLineProgram());
	lineProgram->release();
	pointProgram.reset(buildPointProgram());
	pointProgram->release();

	vao.create();
	vao.bind();
	vbo.reset(buildVertexBuffer());
	vao.release();

	glLineWidth(3.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}


void GLCrossRatioLines::setVertexLine(int line_index, int vertex_index, QVector3D v)
{
	if (line_index == 0)
	{
		for (int i = vertex_index; i < vertices.size() / 2.0f; ++i)
			vertices[i] = v;
	}
	else
	{
		for (int i = vertex_index + vertices.size() / 2.0f; i < vertices.size(); ++i)
			vertices[i] = v;
	}
	updateVertexBuffer();

	/*vbo->bind();
	vbo->write(line_index * 2 * sizeof(QVector3D), &v[0], sizeof(QVector3D) * 2);
	vbo->release();*/
}


void GLCrossRatioLines::reset()
{
	vertices.clear();
	vertices
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(-1.0f, -1.0f, 0.0f);
	updateVertexBuffer();
}



void GLCrossRatioLines::updateVertexBuffer()
{
	vbo->bind();
	vbo->write(0, vertices.constData(), vertices.size() * sizeof(QVector3D));
	vbo->release();
}


void GLCrossRatioLines::render()
{
	//vertices.clear();
	//vertices
	//	<< QVector3D(-0.5f, -0.5f, 0.0f)
	//	<< QVector3D(-0.4f, -0.3f, 0.0f)
	//	<< QVector3D(-0.5f, -0.1f, 0.0f)
	//	<< QVector3D( 0.5f, 0.2f, 0.0f)
	//	<< QVector3D( 0.4f, 0.0f, 0.0f)
	//	<< QVector3D( 0.5f, -0.2f, 0.0f);
	//updateVertexBuffer();

	if (!enabled)
		return;

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	
	vao.bind();
	
	lineProgram->bind();
	f->glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2.0f);
	lineProgram->release();

	lineProgram->bind();
	f->glDrawArrays(GL_LINE_STRIP, vertices.size() / 2.0f, vertices.size() / 2.0f);
	lineProgram->release();
	
	pointProgram->bind();
	f->glDrawArrays(GL_POINTS, 0, vertices.size());
	pointProgram->release();
	
	vao.release();
}



QOpenGLShaderProgram* GLCrossRatioLines::buildLineProgram() const
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


QOpenGLShaderProgram* GLCrossRatioLines::buildPointProgram() const
{
	QOpenGLShaderProgram* prog = new QOpenGLShaderProgram();

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, prog);
	const char *vsrc =
		"attribute highp vec4 in_Vertex;\n"
		"void main(void)\n"
		"{\n"
		"    gl_PointSize = 7.0f;\n"
		"    gl_Position = in_Vertex;\n"
		"}\n";
	vshader->compileSourceCode(vsrc);


	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, prog);
	const char *fsrc =
		"void main(void)\n"
		"{\n"
		"    gl_FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
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



QOpenGLBuffer* GLCrossRatioLines::buildVertexBuffer()
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

