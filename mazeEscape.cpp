#include <sstream> 
/* graphics libraries */
#include "GL/glut.h"
/* audio libraries */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h> 
/********************/
#include "code/maze.cpp"  
#include "code/audioManager.cpp"  
#include "code/lightManager.cpp"
#include "code/drawer.cpp"
#include "code/textureLoader.cpp"  
#include "code/controller.cpp" 

void changeSize(int, int);
void draw();
void updateBar(int); 
void buttonPushed(unsigned char button, int x, int y);    

maze* mazeScheme; 
LightManager* mainLightManager; 
drawer* mainDrawer; 
controller* mainController; 
audioManager* mainAudioManager;  

void changeSize(int w, int h)
{
	glViewport(0, 0, w, h); // it changes the window size
	
	// the projection matrix is modified when the window is resized
	glMatrixMode(GL_PROJECTION); // next matrix operations will be applied to the projection one
	glLoadIdentity(); // it substitutes the matrix with the identity matrix   
	glFrustum(-1, 1, -0.5, 0.5, 0.5, 1000);   
} 

void draw()
	{	     
	if(mazeScheme->checkWin() || (mainController->getDuration() == 0)){ 
			// Set background color
			glDisable(GL_LIGHTING);	 
			glClearColor(0, 0, 0, 0);  
			// Set drawing/text color
			glColor3d(1, 1, 1);  
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0.0, 1.0, 0.0, 1.0);
			glMatrixMode(GL_MODELVIEW); // following operations will be applied to the modelview matrix. Operations of translation, scale and other transformations
			glLoadIdentity(); // identity matrix    
			glRotatef(0, 0, 1, 0); // identity -> rotation matrix	
			glTranslatef(0, 0, 0); // rotation m. -> translation + rotation matrix	  
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // presets the buffer to enable it for color writing and to operate to the depth buffer 
 			if(mazeScheme->checkWin()){
	 			mainController->setWon();
	 		}else 
	 			if(mainController->getDuration() == 0){  
		 			mainController->setLost(mazeScheme);
	 			}
	}else{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // presets the buffer to enable it for color writing and to operate to the depth buffer 
	  
	  	GLfloat X = mainController->getX();
	  	GLfloat Y = mainController->getY();
	  	GLfloat Z = mainController->getZ();
	  	GLfloat A = mainController->getA();

	   	// Spotlight properties including position. 
	   	mainAudioManager->updateListenerPosition(X, Z, A);
		//glLightfv(GL_LIGHT0, GL_POSITION, LightManager::getMat(X, Y, Z));    
	 
		glMatrixMode(GL_MODELVIEW); // following operations will be applied to the modelview matrix. Operations of translation, scale and other transformations
		glLoadIdentity(); // identity matrix   

		glRotatef(-A, 0, 1, 0); // identity -> rotation matrix	
		glTranslatef(-X, -Y, -Z); // rotation m. -> translation + rotation matrix	 

		//useful parameters for ceiling and floor drawing
		int x = mazeScheme->getCols(); 
		int z = mazeScheme->getRows();  
	 
		// material effects and texture enabling 
		glMateriali(GL_FRONT, GL_SHININESS, 32);
		glMaterialfv(GL_FRONT, GL_AMBIENT, LightManager::getMat(1, 1, 1));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, LightManager::getMat(1, 1, 1)); 
		glEnable(GL_TEXTURE_2D);

		//floor
		glMaterialfv(GL_FRONT, GL_SPECULAR, LightManager::getMat(0.8, 0.8, 0.8));
		glBindTexture(GL_TEXTURE_2D, 1);  
		mainDrawer->drawFloor(x, z);

		//ceiling
		glMaterialfv(GL_FRONT, GL_SPECULAR, LightManager::getMat(0.8, 0.8, 0.8));
		glBindTexture(GL_TEXTURE_2D, 2); 
		mainDrawer->drawCeiling(x, z);

		//maze  
		glMaterialfv(GL_FRONT, GL_SPECULAR, LightManager::getMat(1, 1, 1));
		glBindTexture(GL_TEXTURE_2D, 3);  
		mainDrawer->drawMaze(mazeScheme);  
	     
		glDisable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, LightManager::getMat(0.0, 1.0, 0.0)); 
		glMateriali(GL_FRONT, GL_SHININESS, 128); 
		mainDrawer->drawEnabledAlarms(mazeScheme, mainAudioManager); 

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, LightManager::getMat(1.0, 0.0, 0.0)); 
		glMateriali(GL_FRONT, GL_SHININESS,128); 
		mainDrawer->drawDisabledAlarms(mazeScheme); 

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -0.5, 0.5, 0.5, 1000);     

	}
	glutSwapBuffers(); // applied to swap the buffer
}  

void updateBar(int v){  
	if(mainController->getDuration()>0 && !mazeScheme->checkWin()){
    	mainController->updateState(v, mazeScheme);
		glutTimerFunc(100, updateBar, v);  //repost timer 
	} 
} 

void buttonPushed(unsigned char button, int, int){  
	mainController->makeAction(button, mazeScheme, mainAudioManager); 
	glutPostRedisplay(); // sets the displayed window to be updated 
}


int main(int argc, char **argv)
{  
	mazeScheme = new maze();
	mainLightManager = new LightManager(); 
	mainDrawer = new drawer();
	mainController = new controller(2, 0.5, 3, -90);

	glutInit(&argc, argv); // initialize glut
	alutInit(&argc, argv); // initialize alut
	alGetError();
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); // display mode (ex: with single or double buffer)
	glutInitWindowPosition(200, 200); // window position
	glutInitWindowSize(800, 600); // window size
	glutCreateWindow("Maze escape!"); // it creates a window with the specified name
   
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //to set the color buffer 

	glEnable(GL_DEPTH_TEST); // to allow depth updates  
	glCullFace(GL_BACK);  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_CULL_FACE); // enabled for texture application: textures are applied to a specific face based on vertexes definition order (counter clock order for culling).
 
	/* Light loading */
	mainLightManager->lightInit(mainController->getX(), mainController->getY(),mainController->getZ()); 
	mainLightManager->fogInit(); 
	mainAudioManager = new audioManager(mainController->getX(), mainController->getY(),mainController->getZ());  
	/* Texture loading */
	TextureLoader * mainTextureLoader = new TextureLoader();
  	mainTextureLoader->bmpLoader("texture/ground.bmp");  
  	mainTextureLoader->bmpLoader("texture/ceil.bmp");    
	mainTextureLoader->bmpLoader("texture/gBrickWall.bmp", 1024, 1024);   
 
	/* callback functions */	
	// the following method callbacks will be registered and they will be called at each cycle thanks to glutMainLoop 
	glutReshapeFunc(changeSize); // triggered when window size changes
	glutDisplayFunc(draw); // sets the display function callBack. It will be called to redraw the window  
	glutKeyboardFunc(buttonPushed); // triggered when a button is pushed.  
   	glutTimerFunc(100,updateBar,1); // triggered as defined

	glutMainLoop(); // calls registered triggers, it never returns 

	return(0);
}  
 