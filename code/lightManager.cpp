class LightManager{
private: 
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
public:      
	LightManager(){}

	void lightInit(GLfloat X, GLfloat Y,GLfloat Z){  
		this->X = X;
		this->Y = Y;
		this->Z = Z; 
	    // Enable lighting
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0); 
	     
	    GLfloat torchDir[] = { 0.0, 0, -1.0 }; 
	    GLfloat light_ambient[] = { 0.1, 0.1, 0.2, 0.0};
	    glLightfv(GL_LIGHT0, GL_AMBIENT, getMat(0.2, 0.2, 0.2));
	    glShadeModel(GL_SMOOTH); 
	    glLightfv(GL_LIGHT0,GL_SPECULAR,getMat(0, 0, 0, 0));
	    glLightfv(GL_LIGHT0,GL_POSITION,getMat(0, 0, -7, -1));
	    // Definig spotlight attributes
	    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, 8.0);
	    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, 128); 
	    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, torchDir); 
	}

	void fogInit(){
		glFogi(GL_FOG_MODE, GL_EXP); 
		glFogfv(GL_FOG_COLOR, getMat(0.0, 0.0, 0.0));
		glFogf(GL_FOG_DENSITY, 0.1);
		glEnable(GL_FOG);
	}

	static GLfloat* getMat(float x, float y, float z, float other = 1.0f){ // x, y, z can be also R, G, B
		GLfloat* colorMat = new GLfloat[4];
		colorMat[0] = x;
		colorMat[1] = y;
		colorMat[2] = z;
		colorMat[3] = other;
		return colorMat;
	}  
};
 