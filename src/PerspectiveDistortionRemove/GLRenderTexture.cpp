
#include "GLRenderTexture.h"
#include <iostream>


GLRenderTexture::GLRenderTexture(QObject *parent)
	: QObject(parent),
	image(1, 1, QImage::Format::Format_ARGB32)
{
	image.fill(0);
}


GLRenderTexture::~GLRenderTexture()
{
}


void GLRenderTexture::setImage(const QImage& inputImage)
{
	image = inputImage;
	renderFbo.reset(new QOpenGLFramebufferObject(image.size(), renderFbo->format()));
	buildTexture();
}


void GLRenderTexture::buildTexture()
{
	texture.reset(new QOpenGLTexture(image.mirrored()));
	texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Nearest);
	//texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	//texture->setMagnificationFilter(QOpenGLTexture::Linear);
}



void GLRenderTexture::cleanup()
{
	vbo->destroy();
	texture.clear();
	renderFbo.clear();
}


void GLRenderTexture::initialize()
{
	initializeOpenGLFunctions();

	texture.reset(new QOpenGLTexture(image.mirrored()));
	texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);

	program.reset(buildProgram());
	program->release();

	plotProgram.reset(buildPassthorughProgram());
	plotProgram->release();

	vao.create();
	vao.bind();
	vbo.reset(buildQuadTextured());
	vao.release();

	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	renderFbo.reset(new QOpenGLFramebufferObject(image.size(), format));
}



void GLRenderTexture::run()
{
	renderFbo->bind();
	{
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
		f->glViewport(0, 0, renderFbo->width(), renderFbo->height());
		texture->bind();
		vao.bind();
		program->bind();
		f->glDrawArrays(GL_QUADS, 0, 4);
		program->release();
		vao.release();
	}
	renderFbo->bindDefault();
}


void GLRenderTexture::render()
{
	vao.bind();

	if (!texture.isNull())
		texture->bind();

	program->bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glDrawArrays(GL_QUADS, 0, 4);
	program->release();

	vao.release();
}


void GLRenderTexture::plot(int w, int h)
{
	vao.bind();
	plotProgram->bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glActiveTexture(GL_TEXTURE0);
	f->glBindTexture(GL_TEXTURE_2D, renderFbo->texture());
	f->glViewport(0, 0, w, h);
	f->glDrawArrays(GL_QUADS, 0, 4);
	plotProgram->release();
	vao.release();
}



// Build a pass through glsl program
QOpenGLShaderProgram* GLRenderTexture::buildProgram() const
{
	QOpenGLShaderProgram* prog = new QOpenGLShaderProgram();

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, prog);
	const char *vsrc =
		"attribute highp vec4 in_Vertex;\n"
		"attribute mediump vec4 in_TexCoord;\n"
		"varying mediump vec4 texCoord;\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = in_Vertex;\n"
		"    texCoord = in_TexCoord;\n"
		"}\n";
	vshader->compileSourceCode(vsrc);


	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, prog);
	const char *fsrc =
		"uniform sampler2D in_Texture;\n"
		"varying mediump vec4 texCoord;\n"
		"void main(void)\n"
		"{\n"
		"    gl_FragColor = texture2D(in_Texture, texCoord.st);\n"
		"}\n";
	fshader->compileSourceCode(fsrc);

	prog->addShader(vshader);
	prog->addShader(fshader);
	prog->bindAttributeLocation("in_Vertex", 0);
	prog->bindAttributeLocation("in_TexCoord", 1);
	prog->link();
	prog->bind();
	prog->setUniformValue("in_Texture", 0);
	prog->release();

	return prog;
}


// Build a pass-through glsl program
QOpenGLShaderProgram* GLRenderTexture::buildPassthorughProgram() const
{
	QOpenGLShaderProgram* prog = new QOpenGLShaderProgram();

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, prog);
	const char *vsrc =
		"attribute highp vec4 in_Vertex;\n"
		"attribute mediump vec4 in_TexCoord;\n"
		"varying mediump vec4 texCoord;\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = in_Vertex;\n"
		"    texCoord = in_TexCoord;\n"
		"}\n";
	vshader->compileSourceCode(vsrc);


	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, prog);
	const char *fsrc =
		"uniform sampler2D in_Texture;\n"
		"varying mediump vec4 texCoord;\n"
		"void main(void)\n"
		"{\n"
		"    gl_FragColor = texture2D(in_Texture, texCoord.st);\n"
		"}\n";
	fshader->compileSourceCode(fsrc);

	prog->addShader(vshader);
	prog->addShader(fshader);
	prog->bindAttributeLocation("in_Vertex", 0);
	prog->bindAttributeLocation("in_TexCoord", 1);
	prog->link();
	prog->bind();
	prog->setUniformValue("in_Texture", 0);
	prog->release();

	return prog;
}



QOpenGLBuffer* GLRenderTexture::buildQuadTextured() const 
{
	QOpenGLBuffer* lpvbo = new QOpenGLBuffer();

	typedef struct
	{
		float xyz[3];
		float uv[2];
	} VertexUV;

	VertexUV Vertices[] =
	{
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { -1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { 1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } }
	};

	const size_t BufferSize = sizeof(Vertices);
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t UVOffset = sizeof(Vertices[0].xyz);

	// Setup our vertex buffer object.
	lpvbo->create();
	lpvbo->bind();
	lpvbo->allocate(Vertices, BufferSize);

	lpvbo->bind();

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize, 0);
	f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VertexSize, reinterpret_cast<void *>(UVOffset));

	lpvbo->release();

	return lpvbo;
}

