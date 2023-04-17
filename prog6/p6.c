/*
Bitmap Files
Jai Agarwal
ECE 2220 Prog 6
Purpose        - This program's purpose is to read in bitmap files and create new files with alterations
                  such as edge detection and noise application.
Assumptions    - assumes the program is compiled with -lm
Known Bugs     - unsure if the edge detection colors are correct
               - unsure if the box-muller method is calculated properly
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>


/*           Bitmap file structure headers                */
struct HEADER
{  unsigned short int Type;     /* Magic identifier       */
   unsigned int Size;           /* File size in bytes     */
   unsigned short int Reserved1, Reserved2;
   unsigned int Offset;         /* Offset to data (in B)  */
} Header;                       /* -- 14 Bytes --         */

struct INFOHEADER
{  unsigned int Size;           /* Header size in bytes    */
   int Width, Height;           /* Width / Height of image */
   unsigned short int Planes;   /* Number of colour planes */
   unsigned short int Bits;     /* Bits per pixel          */
   unsigned int Compression;    /* Compression type        */
   unsigned int ImageSize;      /* Image size in bytes     */
   int xResolution, yResolution;/* Pixels per meter        */
   unsigned int Colors;         /* Number of colors        */
   unsigned int ImportantColors;/* Important colors        */
} InfoHeader;                   /* -- 40 Bytes --          */
/*                                                         */


// structure that holds the rgb values for a pixel
struct PIXEL{
   unsigned char r, g, b;
};

// this function reads in the header data from a bitmap file
// takes a filepointer to a bitmap file input
// returns nothing
void headerLoad(FILE* inFile);

// writes the header information to the output bitmap
// file pointer to an output file opened in "wb"
// returns nothing
void headerWrite(FILE* outFile);

// does the read mode operation on the bitmap file; writes input file info to outputfil
// takes filepointer to input file, output file, and string of filename
// returns nothing
void readMode(FILE* inFile, FILE* outFile, char* filename);

// (for edge detection) simple check for corner cases to see if coords are in bounds for filterNeighbors
// takes coors of row and col
// returns 1 if the neighbor is valid (exists), else 0 
int validNeighbor(int r, int c);

// (for edge detection) applies the matrix filter to the neighbors of the pixel of focus
// takes the original piixel array, row, and col
// returns a new pixel after applying the matrix filter to originalPixel[r][c]
struct PIXEL filterNeighbors(struct PIXEL** originalPixel, int r, int c);

// does the edge mode operation on the bitmap file; writes to a new bitmap file with the modifications 
// takes filepointer to input file, output file
// returns nothing
void edgeMode(FILE* inFile, FILE* outFile);

// does the noise mode operation on the bitmap file; writes to a new bitmap file with the modifications
// takes filepointer to input file, output file
// returns nothing
void noiseMode(FILE* inFile, FILE* outFile);

// function to create a newfile name with -edge and -noise suffixes
// mode 0 is edge mode and mode 1 is noise
// returns a string with the newfile suffix
char *generateNewName(char *commandLineInput, int mode);

// (for noise) calculates a random number using box-muller method with gaussian distrubution
// is passed a double stdDev
// returns the random number generated
double gaussian(double stdDev);


// main function handles the cmd line args, open files, and calls proper functions
int main(int argc, char **argv){

   if (argc == 1){
      printf("You need to supply some arguments. you can try read, edge, or noise.\n");
      exit(1);
   }
   FILE* inFile = fopen(argv[2], "rb");
   FILE* outFile;
   char* newFileName;

   if (inFile == NULL){
      printf("There was an error opening your file or you didn't supply one. Try again.\n");
      exit(1);
   }

   // load in the header information
   headerLoad(inFile);

   if (strcmp(argv[1], "read") == 0){  // read mode
      if (argv[3] == NULL){
         printf("Please supply an output file!\n");
         exit(1);
      }
      
      outFile = fopen(argv[3], "w");
      readMode(inFile, outFile, argv[2]);

      printf("Done.\nCheck your output file %s.\n", argv[3]);
   }
   else if (strcmp(argv[1], "edge") == 0){   // edge mode
      newFileName = generateNewName(argv[2], 0);
      outFile = fopen(newFileName, "wb");

      edgeMode(inFile, outFile);
      
      printf("Done.\nCheck your output file %s.\n", newFileName);
      free(newFileName);
   }
   else if (strcmp(argv[1], "noise") == 0){  // noise mode
      newFileName = generateNewName(argv[2], 1);
      outFile = fopen(newFileName, "wb");
      
      noiseMode(inFile, outFile);
      
      printf("Done.\nCheck your output file %s.\n", newFileName);
      free(newFileName);
   }
   else {
      printf("Your mode argument is invalid, please supply 'read', 'edge', or 'noise' before your file.\n");
      exit(1);
   }


   fclose(inFile);
   fclose(outFile);
   
}


void headerLoad(FILE* inFile){

   fread(&Header.Type, 2, 1, inFile);
   fread(&Header.Size, 4, 1, inFile);
   fread(&Header.Reserved1, 2, 1, inFile); 
   fread(&Header.Reserved2, 2, 1, inFile); 
   fread(&Header.Offset, 4, 1, inFile);

   fread(&InfoHeader.Size, 4, 1, inFile);
   fread(&InfoHeader.Width, 4, 1, inFile);
   fread(&InfoHeader.Height, 4, 1, inFile);
   fread(&InfoHeader.Planes, 2, 1, inFile);
   fread(&InfoHeader.Bits, 2, 1, inFile);
   fread(&InfoHeader.Compression, 4, 1, inFile);
   fread(&InfoHeader.ImageSize, 4, 1, inFile);
   fread(&InfoHeader.xResolution, 4, 1, inFile);
   fread(&InfoHeader.yResolution, 4, 1, inFile);
   fread(&InfoHeader.Colors, 4, 1, inFile);
   fread(&InfoHeader.ImportantColors, 4, 1, inFile);        
}

void headerWrite(FILE* outFile){

   fwrite(&Header.Type, 2, 1, outFile);
   fwrite(&Header.Size, 4, 1, outFile);
   fwrite(&Header.Reserved1, 2, 1, outFile); 
   fwrite(&Header.Reserved2, 2, 1, outFile); 
   fwrite(&Header.Offset, 4, 1, outFile);

   fwrite(&InfoHeader.Size, 4, 1, outFile);
   fwrite(&InfoHeader.Width, 4, 1, outFile);
   fwrite(&InfoHeader.Height, 4, 1, outFile);
   fwrite(&InfoHeader.Planes, 2, 1, outFile);
   fwrite(&InfoHeader.Bits, 2, 1, outFile);
   fwrite(&InfoHeader.Compression, 4, 1, outFile);
   fwrite(&InfoHeader.ImageSize, 4, 1, outFile);
   fwrite(&InfoHeader.xResolution, 4, 1, outFile);
   fwrite(&InfoHeader.yResolution, 4, 1, outFile);
   fwrite(&InfoHeader.Colors, 4, 1, outFile);
   fwrite(&InfoHeader.ImportantColors, 4, 1, outFile);
}


void readMode(FILE* inFile, FILE* outFile, char* filename){

   unsigned char temp = 0;
   unsigned char padding = (4 - ((3 * InfoHeader.Width) % 4)) % 4;
   unsigned char r, g, b;

   fprintf(outFile, "%s\n", filename);
   fprintf(outFile, "Header.Type = %c\n", Header.Type);
   fprintf(outFile, "Header.Type = %c\n", Header.Type >> 8); // print second character
   fprintf(outFile, "Header.Size = %d\n", Header.Size);
   fprintf(outFile, "Header.Offset = %d\n", Header.Offset); 
   fprintf(outFile, "InfoHeader.Size = %d\n", InfoHeader.Size);
   fprintf(outFile, "InfoHeader.Width = %d\n", InfoHeader.Width);
   fprintf(outFile, "InfoHeader.Height = %d\n", InfoHeader.Height);
   fprintf(outFile, "InfoHeader.Planes = %hd\n", InfoHeader.Planes);
   fprintf(outFile, "InfoHeader.Bits = %hd\n", InfoHeader.Bits);
   fprintf(outFile, "InfoHeader.Compression = %d\n", InfoHeader.Compression);
   fprintf(outFile, "InfoHeader.ImageSize = %d\n", InfoHeader.ImageSize);
   fprintf(outFile, "InfoHeader.xResoltion = %d\n", InfoHeader.xResolution);
   fprintf(outFile, "InfoHeader.yResoltion = %d\n", InfoHeader.yResolution);
   fprintf(outFile, "InfoHeader.Colors = %d\n", InfoHeader.Colors);
   fprintf(outFile, "InfoHeader.ImportantColors = %d\n", InfoHeader.ImportantColors);
   fprintf(outFile, "Padding = %d\n", padding);

   fseek(inFile, 0, SEEK_SET);
   for (int i = 0; i < Header.Offset; i++){
      fread(&temp, 1, 1, inFile);
      fprintf(outFile, "Byte[%d] = %.3d\n", i, temp);
   }

   fseek(inFile, 54, SEEK_SET);
   for (int i = 0; i < InfoHeader.Height; i++){
      for (int j = 0; j < InfoHeader.Width; j++){
         fread(&b, 1, 1, inFile);
         fread(&g, 1, 1, inFile);
         fread(&r, 1, 1, inFile);
         fprintf(outFile, "RGB[%d,%d] = %.3d.%.3d.%.3d\n", i, j, r, g, b);
      }
       for (int k = 0; k < padding; k++){
         // print out the padding at the end of each line
         fread(&temp, 1, 1, inFile);
         fprintf(outFile, "Padding[%d] = %.3d\n", k, temp);
      }
   }

}


int validNeighbor(int y, int x){
   if (x<0 || x>=InfoHeader.Width || y<0 || y>=InfoHeader.Height){
      return 0;
   }
   return 1;
}

struct PIXEL filterNeighbors(struct PIXEL** originalPixel, int r, int c){
  
   struct PIXEL newPixel;
   unsigned char newR = 0, newG = 0, newB = 0;
   char filter[3][3] = 
   {{  0, -1,  0 },
    { -1,  4, -1 },
    {  0, -1,  0 }};

   // i use these offset arrays to add to the coords to check for neighbors
   int changeX[] = {-1, 0, 1, -1, 0, 1, 1, 0, -1}; // for col
   int changeY[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1}; // for row

   for (int i = 0; i < 9; i++){
      if (validNeighbor(r+changeY[i], c+changeX[i])){ // checks if neighbor exists and is in bounds
         newR += filter[0][i] * originalPixel[r+changeY[i]][c+changeX[i]].r;
         newG += filter[0][i] * originalPixel[r+changeY[i]][c+changeX[i]].g;
         newB += filter[0][i] * originalPixel[r+changeY[i]][c+changeX[i]].b;
      }
   }

   newPixel.r = newR;
   newPixel.g = newG;
   newPixel.b = newB;
   return newPixel;

}   


void edgeMode(FILE* inFile, FILE* outFile){

   unsigned char padding = (4 - ((3 * InfoHeader.Width) % 4)) % 4;
   unsigned char paddingtemp;

   struct PIXEL **dataArray = (struct PIXEL**)malloc(InfoHeader.Width * InfoHeader.Height * sizeof(struct PIXEL));
   struct PIXEL **newPixel = (struct PIXEL**)malloc(InfoHeader.Width * InfoHeader.Height * sizeof(struct PIXEL));
   

   fseek(inFile, 54, SEEK_SET);  // skip past the headers
   for (int i = 0; i < InfoHeader.Height; i++){
      dataArray[i] = (struct PIXEL*)malloc(InfoHeader.Width * sizeof(struct PIXEL));
      for (int j = 0; j < InfoHeader.Width; j++){
         fread(&dataArray[i][j].b, 1, 1, inFile);
         fread(&dataArray[i][j].g, 1, 1, inFile);
         fread(&dataArray[i][j].r, 1, 1, inFile);
      }
      for (int k = 0; k < padding; k++){
         fread(&paddingtemp, 1, 1, inFile); 
      }
   }


   for (int r = 0; r < InfoHeader.Height; r++){
      newPixel[r] = (struct PIXEL*)malloc(InfoHeader.Width * sizeof(struct PIXEL));
      for (int c = 0; c < InfoHeader.Width; c++){
         newPixel[r][c] = filterNeighbors(dataArray, r, c);
      }
   }

   // write the header
   headerWrite(outFile);

   // now write the pixels
   for (int i = 0; i < InfoHeader.Height; i++){
      for (int j = 0; j < InfoHeader.Width; j++){
         fwrite(&newPixel[i][j].b, 1, 1, outFile);
         fwrite(&newPixel[i][j].g, 1, 1, outFile);
         fwrite(&newPixel[i][j].r, 1, 1, outFile);
      }
      for (int k = 0; k < padding; k++){
         fwrite(&paddingtemp, 1, 1, outFile);   // write the padding
      }
   }


   for (int i = 0; i < InfoHeader.Height; i++){
      free(dataArray[i]);
      free(newPixel[i]);
   }
   free(dataArray);
   free(newPixel);

}

void noiseMode(FILE* inFile, FILE* outFile){
   
   unsigned char padding = (4 - ((3 * InfoHeader.Width) % 4)) % 4;
   unsigned char paddingtemp;
   double stdDev = 5;

   struct PIXEL **dataArray = (struct PIXEL**)malloc(InfoHeader.Width * InfoHeader.Height * sizeof(struct PIXEL));
   struct PIXEL **newPixel = (struct PIXEL**)malloc(InfoHeader.Width * InfoHeader.Height * sizeof(struct PIXEL));
   
   srand(time(0));

   fseek(inFile, 54, SEEK_SET);  // skip past the headers
   for (int i = 0; i < InfoHeader.Height; i++){
      dataArray[i] = (struct PIXEL*)malloc(InfoHeader.Width * sizeof(struct PIXEL));
      for (int j = 0; j < InfoHeader.Width; j++){
         fread(&dataArray[i][j].b, 1, 1, inFile);
         fread(&dataArray[i][j].g, 1, 1, inFile);
         fread(&dataArray[i][j].r, 1, 1, inFile);
      }
      for (int k = 0; k < padding; k++){
         fread(&paddingtemp, 1, 1, inFile); 
      }
   }

   printf("Enter a standard deviation (5 to 20) for the randomly generated noise: ");
   scanf("%lf", &stdDev);
   while (!(stdDev >= 5 && stdDev <= 20)){
      printf("Please enter a valid number (5 to 20): ");
      scanf("%lf", &stdDev);
   }
   for (int r = 0; r < InfoHeader.Height; r++){
      newPixel[r] = (struct PIXEL*)malloc(InfoHeader.Width * sizeof(struct PIXEL));
      for (int c = 0; c < InfoHeader.Width; c++){
         newPixel[r][c].r = dataArray[r][c].r + gaussian(stdDev);
         newPixel[r][c].g = dataArray[r][c].g + gaussian(stdDev);
         newPixel[r][c].b = dataArray[r][c].b + gaussian(stdDev);
      }
   }

   // write the header
   headerWrite(outFile);

   // now write the pixels
   for (int i = 0; i < InfoHeader.Height; i++){
      for (int j = 0; j < InfoHeader.Width; j++){
         fwrite(&newPixel[i][j].b, 1, 1, outFile);
         fwrite(&newPixel[i][j].g, 1, 1, outFile);
         fwrite(&newPixel[i][j].r, 1, 1, outFile);
      }
      for (int k = 0; k < padding; k++){
         fwrite(&paddingtemp, 1, 1, outFile);   // write the padding
      }
   }


   for (int i = 0; i < InfoHeader.Height; i++){
      free(dataArray[i]);
      free(newPixel[i]);
   }
   free(dataArray);
   free(newPixel);

}

char *generateNewName(char *commandLineInput, int mode){
   char *newFileName = (char*)malloc(strlen(commandLineInput) + 15);
   newFileName = strcpy(newFileName, commandLineInput);
   if (mode == 0)
      strcpy(&newFileName[strlen(commandLineInput)-4], "-edge.bmp");
   else if (mode == 1){
      strcpy(&newFileName[strlen(commandLineInput)-4], "-noise.bmp");
   }
   return newFileName;
}

double gaussian(double stdDev){
   
   // formula is
   // x = Rcos(theta) = sqrt(-2*logU1) * cos (2piU2)
   // theta = 2piU2

   double   R, u1, theta, x;
   double mean = 0;   

   // calculate u1
   u1 = (double)rand() / RAND_MAX;
   // calculate R
   R = sqrt(-2.0 * log(u1));
   // calculate theta
   theta = 2.0 * 3.14159265358979323846 * (double)rand() / RAND_MAX;

   x = (R * cos(theta) * stdDev) + mean;

   return x;
}
