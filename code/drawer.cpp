class drawer{
	private:
		bool firstInizialization;
		bool firstTime;
	public:
		drawer():firstInizialization(true), firstTime(true){}
		void drawFloor(int x, int z){
			glBegin(GL_QUADS); 
			x= x*2;
			z = z*2;
			glNormal3f(0, -1, 0);
			glTexCoord2f(z, z);
			glVertex3f(0, 0, 0);
			glTexCoord2f(0, z);
			glVertex3f(0, 0, z);
			glTexCoord2f(0, 0);
			glVertex3f(x, 0, z);
			glTexCoord2f(z, 0);
			glVertex3f(x, 0, 0);
			glEnd();  
		}

		void drawCeiling(int x, int z){
			glBegin(GL_QUADS); 
			x= x*2;
			z = z*2;
			glNormal3f(0, -1, 0);
			glTexCoord2f(z, 0);
			glVertex3f(x, 1, 0);
			glTexCoord2f(0, 0);
			glVertex3f(x, 1, z);
			glTexCoord2f(0, z);
			glVertex3f(0, 1, z);
			glTexCoord2f(z, z);
			glVertex3f(0, 1, 0);
			glEnd();
		} 

		void drawCube(int x, int y, int z, float factor = 1)
		{

			glBegin(GL_QUADS); 
			// forward face
			glNormal3f(0, 0, 1); // for the shading ( must be done for each face )
			glTexCoord2f(1, 1); // to set texture coordinates
			glVertex3f(x+factor, y, z+factor);
			glTexCoord2f(0, 1);
			glVertex3f(x, y, z+factor);
			glTexCoord2f(0, 0);
			glVertex3f(x, 0, z+factor);
			glTexCoord2f(1, 0);
			glVertex3f(x+factor, 0, z+factor);
			// behind face
			glNormal3f(0, 0, -1);
			glTexCoord2f(1, 1);
			glVertex3f(x, y, z);
			glTexCoord2f(0, 1);
			glVertex3f(x+factor, y, z);
			glTexCoord2f(0, 0);
			glVertex3f(x+factor, 0, z);
			glTexCoord2f(1, 0);
			glVertex3f(x, 0, z);
			// right face
			glNormal3f(1, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3f(x+factor, y, z);
			glTexCoord2f(0, 1);
			glVertex3f(x+factor, y, z+factor);
			glTexCoord2f(0, 0);
			glVertex3f(x+factor, 0, z+factor);
			glTexCoord2f(1, 0);
			glVertex3f(x+factor, 0, z);
			// left face
			glNormal3f(-1, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3f(x, y, z+factor);
			glTexCoord2f(0, 1);
			glVertex3f(x, y, z);
			glTexCoord2f(0, 0);
			glVertex3f(x, 0, z);
			glTexCoord2f(1, 0);
			glVertex3f(x, 0, z+factor);
			// up face
			glNormal3f(0, 1, 0);
			glTexCoord2f(1, 1);
			glVertex3f(x+factor, y, z);
			glTexCoord2f(0, 1);
			glVertex3f(x, 1, z);
			glTexCoord2f(0, 0);
			glVertex3f(x, y, z+factor);
			glTexCoord2f(1, 0);
			glVertex3f(x+factor, y, z+factor);
			// bottom face
			glNormal3f(0, -1, 0);
			glTexCoord2f(1, 1);
			glVertex3f(x, 0, z);
			glTexCoord2f(0, 1);
			glVertex3f(x+factor, 0, z);
			glTexCoord2f(0, 0);
			glVertex3f(x+factor, 0, z+factor);
			glTexCoord2f(1, 0);
			glVertex3f(x, 0, z+factor);
			
			glEnd();
		}

		void drawMaze(maze* mazeScheme){
			float coordX = 0;
			float coordZ = 0;
			bool** matrix = mazeScheme->getMazeMatrix();
			for(int i=0; i<mazeScheme->getRows(); i++){
				for(int j=0; j<mazeScheme->getCols(); j++){
					bool cubeToBeDrawn = matrix[i][j];
					if(cubeToBeDrawn){
						drawCube((coordX+j)*2, 1, (coordZ+i)*2, 2);
					}
				}
			}
		}

		void drawEnabledAlarms(maze* mazeScheme, audioManager* audio){
			float factor = 0.1;
			for(int i=0; i<mazeScheme->getAlarmNum(); i++){
				if(mazeScheme->getAlarmListElem(i).isEnabled()){ 
					int x = mazeScheme->getAlarmListElem(i).getAlarmCoordX()*2+1+factor;
					int y = 1;
					int z = mazeScheme->getAlarmListElem(i).getAlarmCoordZ()*2+1+factor;
					drawCube(x, y, z, factor); 
					if(firstInizialization){ 
						audio->loadAudio("audio/alarm.wav", x, z); 
						if(firstTime){
							firstTime = false;
							audio->loadAudio("audio/alarm.wav", x, z); 
						} 
					}
				}
			}
			firstInizialization = false;
		}

		void drawDisabledAlarms(maze* mazeScheme){
			float factor = 0.1;
			for(int i=0; i<mazeScheme->getAlarmNum(); i++){
				if(!mazeScheme->getAlarmListElem(i).isEnabled()){ 
					int x = mazeScheme->getAlarmListElem(i).getAlarmCoordX()*2+1+factor;
					int y = 1;
					int z = mazeScheme->getAlarmListElem(i).getAlarmCoordZ()*2+1+factor;
					drawCube(x, y, z, factor);  
				}
			}
		}

}; 