#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

int* createSeam(int length) {
  int* seam = new int[length];
  for(int i =0; i< length;i++){
    seam[i]=0;
  }
  int *ptr = seam; 

  return ptr;
}

void deleteSeam(int* seam) {
  delete[] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  fstream ifs(filename);
  if(!(ifs.is_open())){
    cout << "Error: failed to open input file - " << filename << endl;
    return false; 
  }
  string fline;
  ifs>> fline; 
  
  int w, h; 
  ifs >> w >> h; 
  if(ifs.fail()){
    cout<< "Error: read non-integer value"<<endl; 
    return false; 
  }
   if(w != width){
      cout << "Error: input width ("<< width << ") does not match value in file (" << w << ")"<< endl;
      return false; 
    }
    if(h != height){
      cout << "Error: input height ("<< height << ") does not match value in file (" << h << ")"<< endl;
      return false; 
    }
    if(fline != "P3" && fline != "p3"){
      cout << "Error: type is "<< fline << " instead of P3"<< endl;
      return false; 
    }

  int pixel; 
  ifs>> pixel; 
  int red, green, blue;
  int i;
  int cnt = 0; 
  for(i = 0; i< height; i++){
    for(int j =0; j < width; j++){
      if(ifs.eof()){
      break; 
    }
      cnt +=3; 
      ifs>> red;
      if(ifs.fail() && !ifs.eof()){
        cout << "Error: read non-integer value"<<endl; 
        return false; 
      }
      if(ifs.fail() && ifs.eof()){
        cout << "Error: not enough color values"<<endl; 
        return false; 
      }
      ifs>> green;
      if(ifs.fail() && !ifs.eof()){
        cout << "Error: read non-integer value"<<endl; 
        return false; 
      }
      if(ifs.fail() && ifs.eof()){
        cout << "Error: not enough color values"<<endl; 
        return false; 
      }
      ifs>> blue;
    if(ifs.fail() && !ifs.eof()){
        cout << "Error: read non-integer value"<<endl; 
        return false; 
      }
      if(ifs.fail() && ifs.eof()){
        cout << "Error: not enough color values"<<endl; 
        return false; 
      }
    
      if(red < 0 || red > 255){
        cout << "Error: invalid color value " << red << endl;
        return false; 
      }
     
      if(green < 0|| green > 255){
        cout << "Error: invalid color value " << green << endl;
        return false; 
      }
     
      if(blue < 0 || blue > 255){
        cout << "Error: invalid color value " << blue << endl;
        return false; 
      }

      image[j][i].r = red;
      image[j][i].g = green;
      image[j][i].b = blue;     
    }
    
  }
  int last_space = 0; 
  ifs>> last_space;
  if(!ifs.fail()){
    cout << "Error: too many color values";
    return false;
  }
  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
  ofstream ofs(filename);
  if(!ofs.is_open()){
    return false; 
  }
  ofs << "P3"<< endl;
  ofs<< width << " " << height << endl; 
  ofs << 255<< endl; 
  for(int i = 0; i < height; i++){
    for(int j = 0; j< width; j++){
      ofs << image[j][i].r << " " << image[j][i].g << " " << image[j][i].b << " ";
    }
  }

  return true;
}

int energy(Pixel** image, int x, int y, int width, int height) { 
  int num =0;

  int x_gradient=0; 
  int y_gradient = 0;
  // image[width][height]
  if(height == 1){
    return 0; 
  }
  //top left
  if(x==0 && y == 0){
    x_gradient = pow(image[x+1][y].r - image[width-1][y].r, 2) +pow(image[x+1][y].g - image[width-1][y].g, 2) + pow(image[x+1][y].b - image[width-1][y].b, 2);
    y_gradient = pow(image[x][y+1].r - image[x][height-1].r, 2) + pow(image[x][y+1].g - image[x][height-1].g, 2) + pow(image[x][y+1].b - image[x][height-1].b, 2);
    num = x_gradient+y_gradient;
    return num; 
  }
  //left
  if(x == 0 && y>0 && y!= height-1){
    x_gradient = pow(image[x+1][y].r - image[width-1][y].r, 2) +pow(image[x+1][y].g - image[width-1][y].g, 2) + pow(image[x+1][y].b - image[width-1][y].b, 2);
    y_gradient = pow(image[x][y+1].r - image[x][y-1].r, 2) + pow(image[x][y+1].g - image[x][y-1].g, 2) + pow(image[x][y+1].b - image[x][y-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }
  //bottom left
  if(x==0 && y == height-1){
    x_gradient = pow(image[x+1][y].r - image[width-1][y].r, 2) +pow(image[x+1][y].g - image[width-1][y].g, 2) + pow(image[x+1][y].b - image[width-1][y].b, 2);
    y_gradient = pow(image[x][0].r - image[x][y-1].r, 2) + pow(image[x][0].g - image[x][y-1].g, 2) + pow(image[x][0].b - image[x][y-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }
  //top
  if(x>0 && y ==0 && x<width-1){
    x_gradient = pow(image[x+1][y].r - image[x-1][y].r, 2) +pow(image[x+1][y].g - image[x-1][y].g, 2) + pow(image[x+1][y].b - image[x-1][y].b, 2);
    y_gradient = pow(image[x][y+1].r - image[x][height-1].r, 2) + pow(image[x][y+1].g - image[x][height-1].g, 2) + pow(image[x][y+1].b - image[x][height-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }
  //top right
  if(x == width-1 && y == 0){
    x_gradient = pow(image[0][y].r - image[x-1][y].r, 2) +pow(image[0][y].g - image[x-1][y].g, 2) + pow(image[0][y].b - image[x-1][y].b, 2);
    y_gradient = pow(image[x][y+1].r - image[x][height-1].r, 2) + pow(image[x][y+1].g - image[x][height-1].g, 2) + pow(image[x][y+1].b - image[x][height-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }
  //right
  if(x== width-1 && y >0 && y != height-1){
    x_gradient = pow(image[0][y].r - image[x-1][y].r, 2) +pow(image[0][y].g - image[x-1][y].g, 2) + pow(image[0][y].b - image[x-1][y].b, 2);
    y_gradient = pow(image[x][y+1].r - image[x][y-1].r, 2) + pow(image[x][y+1].g - image[x][y-1].g, 2) + pow(image[x][y+1].b - image[x][y-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }
  //bottom right
  if(x == width-1 && y == height-1){
    x_gradient = pow(image[0][y].r - image[x-1][y].r, 2) +pow(image[0][y].g - image[x-1][y].g, 2) + pow(image[0][y].b - image[x-1][y].b, 2);
    y_gradient = pow(image[x][0].r - image[x][y-1].r, 2) + pow(image[x][0].g - image[x][y-1].g, 2) + pow(image[x][0].b - image[x][y-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  } 
  //bottom
  if(x>0 && y == height-1 && x< width-1){
    x_gradient = pow(image[x+1][y].r - image[x-1][y].r, 2) +pow(image[x+1][y].g - image[x-1][y].g, 2) + pow(image[x+1][y].b - image[x-1][y].b, 2);
    y_gradient = pow(image[x][0].r - image[x][y-1].r, 2) + pow(image[x][0].g - image[x][y-1].g, 2) + pow(image[x][0].b - image[x][y-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }
  else{
    x_gradient = pow(image[x+1][y].r - image[x-1][y].r, 2) +pow(image[x+1][y].g - image[x-1][y].g, 2) + pow(image[x+1][y].b - image[x-1][y].b, 2);
    y_gradient = pow(image[x][y+1].r - image[x][y-1].r, 2) + pow(image[x][y+1].g - image[x][y-1].g, 2) + pow(image[x][y+1].b - image[x][y-1].b, 2);
    num = x_gradient+y_gradient;
    return num;
  }

  return 0;
}

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {

  //image[width][row]
  seam[0] = start_col;
  int sum =0; 
  int j = start_col;
  if(width == 1 && height == 1){
    return 0; 
  }
  if(width == 1){
    for(int d =0; d < height; d++){
      if(d== 0){
        sum += pow(image[0][d+1].r - image[0][height-1].r, 2) + pow(image[0][d+1].g - image[0][height-1].g, 2) + pow(image[0][d+1].b - image[0][height-1].b, 2);
      }
      else if(d>0 && d<height-1){
        sum += pow(image[0][d+1].r - image[0][d-1].r, 2) + pow(image[0][d+1].g - image[0][d-1].g, 2) + pow(image[0][d+1].b - image[0][d-1].b, 2);
      }
      else{
        sum += pow(image[0][0].r - image[0][d-1].r, 2) + pow(image[0][0].g - image[0][d-1].g, 2) + pow(image[0][0].b - image[0][d-1].b, 2);

      }

    }
    seam[0] = 0;
    return sum;
  }
  sum += energy(image,j,0,width, height);
  for(int i =1; i < height; i++){
    if(j == 0){
      if(energy(image,j,i,width, height) <= energy(image,j+1,i,width,height)){
        j = j;
        seam[i] = j;      
        sum += energy(image,j,i,width,height);
      }
      else{
        j = j + 1;
        seam[i] = j;      
        sum += energy(image,j,i,width,height);
      }
    }
    else if(j == width-1){
      if(energy(image,j-1,i,width, height) < energy(image,j,i,width,height)) {
        j = j-1;
        seam[i] = j; 
        sum += energy(image,j,i,width,height);
      }
      else if(energy(image,j-1,i,width, height) >= energy(image,j,i,width,height)){
        j = j;
        seam[i] = j; 
        sum += energy(image,j,i,width,height);
      }
    }
    else {
      if(energy(image,j,i,width, height) <= energy(image,j+1,i,width,height) && energy(image,j,i,width, height) <= energy(image,j-1,i,width,height)){
        j = j;
        seam[i] = j; 
        sum += energy(image,j,i,width,height);
      }
      else if(energy(image,j+1,i,width, height) < energy(image,j,i,width,height) && energy(image,j+1,i,width, height) <= energy(image,j-1,i,width,height)){
        j = j +1;
        seam[i] = j; 
        sum += energy(image,j,i,width,height);
      }
      else{
        j = j - 1;
        seam[i] = j; 
        sum += energy(image,j,i,width,height);
      }
    }
    
    
    // if(j == 0){
    //   if(energy(image,j,i,width,height) <= energy(image,j+1,i,width,height)){
    //     j = j;
    //     seam[i] = j;      
    //     sum += energy(image,j,i,width,height);
    //   }
    //   if(energy(image,j,i,width,height) > energy(image,j+1,i,width,height) ){
    //     j = j+1;
    //     seam[i] = j; 
    //     sum += energy(image,j,i,width,height);
    //   }
    //   continue;
    // }
    // if(j == width-1){
    //   if(energy(image,j,i,width,height) <= energy(image,j-1,i,width,height) ){
    //     j = j;
    //     seam[i] = j; 
    //     sum += energy(image,j,i,width,height);
    //   }
    //   if(energy(image,j,i,width,height) > energy(image,j-1,i,width,height) ){
    //     j = j - 1;
    //     seam[i] = j; 
    //     sum += energy(image,j,i,width,height);
    //   }
    //   continue;
    // }
    // else{
    //   if(energy(image,j,i,width,height) <= energy(image,j+1,i,width,height) && energy(image,j,i,width,height) <= energy(image,j-1,i,width,height) ){
    //     j = j;
    //     seam[i] = j; 
    //     sum += energy(image,j,i,width,height);
    //   }
    //   else if(energy(image,j,i,width,height) > energy(image,j+1,i,width,height)){
    //     j = j + 1;
    //     seam[i] = j; 
    //     sum += energy(image,j,i,width,height);
    //   }
    //   else if(energy(image,j+1,i,width,height) >= energy(image,j-1,i,width,height)  ){
    //     j = j - 1;
    //     seam[i] = j; 
    //     sum += energy(image,j,i,width,height);
    //   }
    //   continue;
    // }
  }

  return sum;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
  cout<< image << start_row << width << height << seam;
  return 0;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
  int* createdSeam1 = createSeam(height);
  int col1 = loadVerticalSeam(image, 0, width, height, createdSeam1);
  for(int i =1; i< width; i++){
    int* createdSeam2 = createSeam(height);
    int col2 = loadVerticalSeam(image, i, width, height, createdSeam2);
    if(col1 <= col2){
      deleteSeam(createdSeam2);
    }
    else if(col2 < col1){
      deleteSeam(createdSeam1);
      createdSeam1 = createdSeam2;
      col1 = col2; 
    }
    else{
      deleteSeam(createdSeam2);
    }
  }

  return createdSeam1;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
  cout << image << width << height;
  return nullptr;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
  for(int i =0; i < height; i++){
    for(int j =verticalSeam[i]; j < width-1; j++){
      image[j][i].r = image[j+1][i].r;
      image[j][i].g = image[j+1][i].g;
      image[j][i].b = image[j+1][i].b;
    }
  }
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
  cout << image << width << height << horizontalSeam;
}
