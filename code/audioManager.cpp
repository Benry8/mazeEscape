#include <string>
#include <vector>
using namespace std;  
 
class audioManager{
	private: 
    	vector<ALuint*>* buffer;
    	vector<ALuint*>* source;
    	ALCdevice* device; 
    	ALCcontext* context;
    	ALfloat y; // stored one time, it doesn't need to be updated
    	ALuint Abuffer;
    	ALuint Asource;

    	void loadEnvAudio(ALfloat x, ALfloat y, ALfloat z){
    		string file = "audio/thump.wav";
			ALuint Abuffer = alutCreateBufferFromFile(file.c_str());  
			ALuint Asource;
			alGenSources(1, &Asource);    
			alSourcef(Asource, AL_PITCH, 0.3);//0.3);
			alSourcef(Asource, AL_GAIN, 0.3);
			alSource3f(Asource, AL_POSITION, x, y, z);
			alSource3f(Asource, AL_VELOCITY, 0, 0, 0);
			alSourcei(Asource, AL_LOOPING, AL_TRUE);  

			//attach a source to a buffer
			alSourcei(Asource, AL_BUFFER, Abuffer);
			
			//to make it play
			alSourcePlay(Asource); 
    	}
	public:
		audioManager(ALfloat x, ALfloat y, ALfloat z){ 
			this->y = y;
    		buffer = new vector<ALuint*>;
    		source = new vector<ALuint*>;
			/* create context */
			device = alcOpenDevice(NULL);
			context = alcCreateContext(device, NULL); 
			alcMakeContextCurrent(context);
			alListener3f(AL_POSITION, x, y, z);
			alListener3f(AL_VELOCITY, 0, 0, 0);  
			ALfloat aVector[6] = { 0, 0, -1, 0, 1, 0};
			alListenerfv(AL_ORIENTATION, aVector); 
			//environment audio
			loadEnvAudio(x, y, z); 
		}

		void updateListenerPosition(ALfloat x, ALfloat z, ALfloat angle){ 
			alListener3f(AL_POSITION, x, y, z);  
			alSource3f(Asource, AL_POSITION, x, y, z);
			ALfloat xDirection = (float) cos(angle);
			ALfloat zDirection = (float) sin(angle);
			ALfloat aVector[6] = {xDirection, 0, zDirection, 0, 1, 0};
			alListenerfv(AL_ORIENTATION, aVector); 
		}
 
		void loadAudio(string file, ALfloat x, ALfloat z){ 
			ALuint* auxBuffer = new ALuint(alutCreateBufferFromFile(file.c_str()));
			buffer->push_back(auxBuffer);
			int i = source->size();
			ALuint *auxSource = new ALuint();
			alGenSources(1, auxSource);  
			source->push_back(auxSource); 
			alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED); // exponential curve of sound attenuation
			alSourcef(*source->at(i), AL_PITCH, 1);
			alSourcef(*source->at(i), AL_GAIN, 1);
			alSource3f(*source->at(i), AL_POSITION, x, y, z);
			alSource3f(*source->at(i), AL_DIRECTION, 0, 1, 0);
			alSource3f(*source->at(i), AL_VELOCITY, 0, 0, 0);   
			alSourcei(*source->at(i), AL_LOOPING, AL_TRUE);   
			alSourcef(*source->at(i), AL_REFERENCE_DISTANCE, 3); 
			alSourcef(*source->at(i), AL_ROLLOFF_FACTOR, 5); 
			alSourcef(*source->at(i), AL_MIN_GAIN, 0.001); 
			//attach a source to a buffer
			alSourcei(*source->at(i), AL_BUFFER, *buffer->at(i));
			
			//to make it play
			alSourcePlay(*source->at(i));  
		}
 
		void stopAudioSource(int i){
			// to close sources
			i=i+1;
			alSourceStop(*source->at(i)); 
		}

		void closeAll(){  
			for(int i =0; i<buffer->size(); i++){
				alDeleteSources(1, source->at(i));
				alDeleteBuffers(1, buffer->at(i));  
			}
			alDeleteSources(1, &Asource);
			alDeleteBuffers(1, &Abuffer);  
			alcDestroyContext(context);
			alcCloseDevice(device);
			alutExit();
		}

}; 
