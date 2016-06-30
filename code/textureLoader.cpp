#include <fstream> 

class TextureLoader{
	private:
		unsigned char* data; // Actual RGB data
		int textureNumber;

		bool textureBinder(unsigned char* texture, int x = 256, int y = 256){ 
			textureNumber++;
			glBindTexture(GL_TEXTURE_2D, textureNumber);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_BGR, GL_UNSIGNED_BYTE, texture);
			return true;
		}
	public: 
		TextureLoader(): textureNumber(0){} 

		GLint bmpLoader(const char* fileName, unsigned int width = 256, unsigned int height = 256){  
			unsigned char header[54]; // Each BMP file begins by a 54-bytes header
			unsigned int dataPos;     // Position in the file where the actual data begins
			//unsigned int width, height;
			unsigned int imageSize;   // = width*height*3 
			FILE * file = fopen(fileName,"rb");
			if (!file){printf("Image could not be opened\n"); return 0;}

			if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
	    		printf("Not a correct BMP file\n");
	    		return false;
			}

			if ( header[0]!='B' || header[1]!='M' ){
			    printf("Not a correct BMP file\n");
			    return 0;
			}

			dataPos    = *(int*)&(header[0x0A]);
			imageSize  = *(int*)&(header[0x22]);
			width      = *(int*)&(header[0x12]);
			height     = *(int*)&(header[0x16]);
			// Some BMP files are misformatted, guess missing information
			if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
			if (dataPos==0)      dataPos=54; // The BMP header is done that way

			// Create a buffer
			data = new unsigned char [imageSize];

			// Read the actual data from the file into the buffer
			fread(data,1,imageSize,file);

			//Everything is in memory now, the file can be closed
			fclose(file);
			
			//it binds a texture
			textureBinder(data, width, height);
			
			return 0;
		}
};
