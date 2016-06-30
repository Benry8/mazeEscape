#include <fstream>
#include <string> 
#include <vector>
#include <cmath>
using namespace std;


class alarm{
	private:
		int alarmCoordX;
		int alarmCoordZ;
		bool enabled;
	public:
		alarm(int alarmCoordX = 0, int alarmCoordZ = 0): enabled(true){
			this->alarmCoordX = alarmCoordX;
			this->alarmCoordZ = alarmCoordZ;
		}

		int getAlarmCoordX(){
			return alarmCoordX;
		}

		int getAlarmCoordZ(){
			return alarmCoordZ;
		}

		bool isEnabled(){
			return enabled;
		}

		bool disableAlarm(){
			enabled = false;
			return enabled;
		} 
}; 

class maze{
	private:
		int rows;
		int cols;
		bool ** mazeMatrix;
		long unsigned int alarmNum;
		int enabledAlarms;
		vector<alarm> alarmList;
		bool win;
	public:  
		maze(){
			win = false;
			ifstream file;
			file.open("maze.txt");
			string value;
			file>>rows;
			file>>cols; 
			rows=rows+2; // borders
			cols=cols+2; // borders
			mazeMatrix = new bool*[rows];
			for(int i = 0; i < rows; ++i)
				mazeMatrix[i] = new bool[cols];
			/* Default values inizialization: the maze is always closed: border = 2 */
			int i=0;
			int j=0;
			for(i=0; i<rows; i++){
				for(j=0; j<cols; j++){
					if(i==0 || i==rows-1 || j==0 || j==cols-1){
						mazeMatrix[i][j] = 1;
					}else{
						mazeMatrix[i][j] = 0;
					}
				}
			} 
			string line;
			
			for(i=1; i<rows-1; i++){ 
				for(j=1; j<cols-1; j++){
					file >> mazeMatrix[i][j];
				}
			}  

			// now will be read from file the number of alarms columns and their positions 
			file>>alarmNum;
			enabledAlarms = alarmNum; // it will be used to indicate if the player has winned or not
			alarmList.resize(alarmNum);  
			for(i=0; i<alarmList.size(); i++){
				int x, k;
				file>>x;
				file>>k;
				alarmList[i] = alarm(x+1, k+1); // +1 because there is a wall among the whole maze 
			}

			file.close(); 
		} 


		int getRows(){
			return rows;
		}

		int getCols(){
			return cols;
		}

		int getAlarmNum(){
			return alarmNum;
		}		
		
		int getEnabledAlarmNum(){
			return enabledAlarms;
		}

		int getDisabledAlarmNum(){
			return alarmNum - enabledAlarms;
		}

		bool** getMazeMatrix(){
			return mazeMatrix;
		}

		vector<alarm> getAlarmList(){
			return alarmList;
		}


		alarm getAlarmListElem(int k){
			return alarmList.at(k);
		}

		bool isWall(int z, int x){   
			return mazeMatrix[z][x];
		}

		bool isColliding(int z, int x, float directionZ, float directionX){  
			bool collision = false;
			bool upWall = false; 
			bool leftWall = false; 
			bool rightWall = false;
			bool bottomWall = false; 
			bottomWall = isWall((z/2), (x-1)/2) || isWall(((z/2)), ((x/2)));  
			collision = (bottomWall && (directionZ>0));

			if(!collision && x>0){  
				leftWall = isWall((z-1)/2, (x-1)/2) || isWall(((z)/2), ((x-1)/2));
				collision = (leftWall && (directionX<0));	
			} 
			if(!collision && z>0){ 
				upWall = isWall((z-1)/2, (x-1)/2) || isWall(((z-1)/2), ((x)/2)); 
				collision = (upWall && (directionZ<0));   
			}
			if(!collision){ 
				rightWall = isWall((z-1)/2, (x)/2) || isWall(((z)/2), ((x)/2)); 
				collision = (rightWall && (directionX>0));	 
			}   
			return collision;
		}

		static float getDistance(float z1, float x1, float z2, float x2){
			return sqrt(((z1-z2)*(z1-z2))+((x1-x2)*(x1-x2))); // coordinates always positive for construction.
		}

		bool checkWin(){
			return win;
		}

		int disableNearAlarm(int z, int x){
			int disabledIndex = -1; // -1 = no alarms disabled
			int acceptedDistance = 2; 
			for(int i=0; (disabledIndex<0) && i<alarmNum; i++){
				if(alarmList[i].isEnabled()){
					if(getDistance(z, x, alarmList[i].getAlarmCoordZ()*2+1, alarmList[i].getAlarmCoordX()*2+1) < acceptedDistance){
						alarmList[i].disableAlarm();
						enabledAlarms--;
						disabledIndex = i;
					}
				}
			}
			if(enabledAlarms == 0){
				win = true;
			} 
			return disabledIndex;  
		}
}; 