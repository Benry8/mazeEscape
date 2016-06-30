#define SPACEBAR 32
#define ESCKEY 27  
#include <ctime>

const double PI = std::atan(1.0)*4;

// manager of user commands. It also establish if the user has won or lost
class controller{
	private:
		GLfloat X;
		GLfloat Y;
		GLfloat Z;
		GLfloat A;
		int counter;
		int duration; 
		bool won;
		bool lost;

		string getOrientation(){ 
			int angle = ((int)abs(A))%360; 
			if(angle == 0)
				return "North";
			if(angle == 90)
				return "East";
			if(angle == 180)
				return "South";
			if(angle == 270)
				return "West"; 
		}

	public:  
		controller(GLfloat X, GLfloat Y, GLfloat Z, GLfloat A=-90){
			this->X = X;
			this->Y = Y;
			this->Z = Z;
			this->A = A;
			duration = 100;
			counter = 0;
			won = false;
			lost = false;
		}

		void printbitmap(const string msg, double x, double y)
		{
		   glRasterPos2d(x, y);
		   for (string::const_iterator ii = msg.begin(); ii != msg.end(); ++ii)
		   {
		      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ii);
		   }
		}

		void makeAction(unsigned char button, maze * mazeScheme, audioManager* audio){   
			if(!won && !lost){
				if((button == 'a')||(button == 'q')) A += 90;  
				if((button == 'd')||(button == 'e')) A -= 90; 
				if(button == 'w') move(-1, mazeScheme);
				if(button == 's') move(1, mazeScheme);  
				if(button == SPACEBAR) disableAlarm(mazeScheme, audio); 
			}else
				if(button == ESCKEY) {
				audio->closeAll();
				exit(0); // quit
				}
		}

		void updateState(int v, maze* mazeScheme){  
		    counter++;
		    if(counter == 10){
		       	duration--;
		    	counter = 0;
		    } 
			stringstream timeCounter; // stringstream used for the conversion 
			timeCounter << duration;//add the value of Number to the characters in the stream  
		    string text = "Maze escape! - time: "+timeCounter.str()+"/100 - Orientation: "+getOrientation(); 
			stringstream disabledAlarms; // stringstream used for the conversion 
			disabledAlarms<< mazeScheme->getDisabledAlarmNum();
		    text+= " - Disabled Alarms: "+ disabledAlarms.str() +"/"; 
			stringstream alarms; // stringstream used for the conversion 
			alarms<<mazeScheme->getAlarmNum();
		    text+= alarms.str();
			glutSetWindowTitle( (text.c_str()));
		}

		void move(float incr, maze * mazeScheme){    
			float possibleZ =(round) (Z + incr*(cos(A * PI / 180.0 )));
			float possibleX =(round) (X + incr*(sin(A * PI / 180.0 )));   
			if(!mazeScheme->isColliding(possibleZ, possibleX, possibleZ-Z, possibleX-X)){  
				Z = possibleZ;
				X = possibleX;  
			}
		} 

		void disableAlarm(maze * mazeScheme, audioManager* audio){
			int roundZ = (int) round(Z);
			int roundX = (int) round(X);  
			int disabledIndex = mazeScheme->disableNearAlarm(roundZ, roundX);
			if(disabledIndex>=0) 
				audio->stopAudioSource(disabledIndex);  
		}

		int getDuration(){
			return duration;
		}

		GLfloat getX(){
			return X;
		}

		GLfloat getY(){
			return Y;
		}

		GLfloat getZ(){
			return Z;
		}

		GLfloat getA(){
			return A;
		}

		void setWon(){ 
			won = true;   
			stringstream record; // stringstream used for the conversion 
			record << getDuration();//add the value of Number to the characters in the stream  
		    string message = "You have won! Remaining time: "+record.str()+ "/100";  
		  	printbitmap("Press 'Esc' to Quit", 0.05, 0.95);  
			printbitmap(message, 0.3, 0.5); // time string 
			string text="Won";
			glutSetWindowTitle( (text.c_str())); 
		}

		void setLost(maze* mazeScheme){ 
			lost = true;  
			stringstream record; // stringstream used for the conversion 
			record << mazeScheme->getEnabledAlarmNum(); 
			stringstream tot; // stringstream used for the conversion 
			tot << mazeScheme->getAlarmNum(); 
		    string message = "Remaining alarms to disable: "+record.str()+ "/"+tot.str();  
		  	printbitmap("Press 'Esc' to Quit", 0.05, 0.95);  
			printbitmap("Game Over!", 0.3, 0.7); // time string
			printbitmap(message, 0.3, 0.5); // time string			
			string text="Game Over";
			glutSetWindowTitle((text.c_str())); 
		} 
};