
#include "GLLines.h"
#include <iostream>


GLLines::GLLines(int line_count,QObject *parent)
	: QObject(parent),
	enabled(true),
	maxLines(line_count * 2)
{
	for (int i = 0; i < maxLines; ++i)
		vertices.push_back(QVector3D(-1.0f, -1.0f, 0.0f));
}


GLLines::~GLLines()
{
}

void GLLines::onEnable(bool enable)
{
	enabled = enable;
}



void GLLines::cleanup()
{
	vbo->destroy();
}


void GLLines::initialize()
{
	initializeOpenGLFunctions();
	

	program.reset(buildProgram());
	program->release();

	vao.create();
	vao.bind();
	vbo.reset(buildVertexBuffer());
	vao.release();
}


void GLLines::setVertexLine(int line_index, int vertex_index, QVector3D v)
{
	if (vertex_index == 0)
	{
		vertices[line_index * 2 + 0] = v;	// line_begin
		vertices[line_index * 2 + 1] = v;	// line_end
	}
	else
	{
		vertices[line_index * 2 + 1] = v;	// line_end
	}
		

	updateVertexBuffer();

	/*vbo->bind();
	vbo->write(line_index * 2 * sizeof(QVector3D), &v[0], sizeof(QVector3D) * 2);
	vbo->release();*/
}



void GLLines::setVertices(int vertex_index, QVector3D v)
{
	vertices[vertex_index] = v;	
	updateVertexBuffer();
}


void GLLines::computeCanonicalVertices(int width, int height)
{
	for (int i = 0; i < maxLines; ++i)
	{
		QVector3D v = vertices[i];
		vertices[i].setX(v.x() / float(width) * 2.0f - 1.0f);
		vertices[i].setY(v.y() / (float(height) * 2.0f - 1.0f) * (-1.0f)); 	// inverting y coordinate
	}

	updateVertexBuffer();
}


void GLLines::reset()
{
	vertices.clear();
	for (int i = 0; i < maxLines; ++i)
		vertices.push_back(QVector3D(-1.0f, -1.0f, 0.0f));
	updateVertexBuffer();
}



void GLLines::updateVertexBuffer()
{
	vbo->bind();
	vbo->write(0, vertices.constData(), vertices.size() * sizeof(QVector3D));
	vbo->release();
}


void GLLines::render()
{
	if (!enabled)
		return;

	vao.bind();
	program->bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glDrawArrays(GL_LINES, 0, vertices.size());
	program->release();
	vao.release();
}



QOpenGLShaderProgram* GLLines::buildProgram() const
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



QOpenGLBuffer* GLLines::buildVertexBuffer()
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

