#ifndef gomoku_game
#define gomoku_game

#include <iostream>
#include <cmath>

// Defining pieces
#define MT 0 // Empty
#define BK 1 // Black
#define WH 2 // White

#define BK_char 'X'
#define WH_char 'O'
#define MT_char '.'
#define NT_char '*'

class board{
public:
    int lc[4] = {0,0,0,0};
    int* grid;
    float center_x=7;
    float center_y=7;
    float* val;
    const int h = 15;
    const int w = 15;
    int turn = 0;

  board(){
    grid = (int*)new int[h*w];
    val = (float*)new float [h*w];
    for(int i = 0; i<h; i++){
      for(int j = 0; j<w; j++){
        grid[i*h+j] = MT;
        val[i*h+j] = 0;
      }
    }
  }

  void purge(){
    delete[] grid;
    delete[] val;
    delete[] lc;
  }

  // Force setting a position's piece
  void set(int x, int y, int p){
    grid[y*h+x] = p;
  }

  // Placing a piece if the position is open
  int place(int x, int y, int p){
    if(!place_perm(x, y)){
      return 1;
    }
    if(grid[y*h+x] != MT || p == MT){
      return 1;
    }
    else{
      grid[y*h+x] = p;
      turn++;
      val[y*h+x] = pos_eval(x, y);
      center_x = (center_x*turn+x)/(turn+1);
      center_y = (center_y*turn+y)/(turn+1);
      return 0;
    }
  }


  float get_val(int x, int y, int p){
    if(x<0 || y<0 || x>14 || y>14){
      return 0;
    }
    if(p==lookup(x, y)){
      return val[y*h+x];
    }
    else{
      return -val[y*h+x]*1.3;
    }
  }

  // Lookup the status of a position
  int lookup(int x, int y){
    return grid[y*h+x];
  }

  int place_perm(int x, int y){
    if(lookup(x, y)!=MT){return 0;}
    else{
      if(this->turn == 0 && !(y==h/2 && x==w/2)){
        return 0;
      }
      if(this->turn == 1 && !(y==h/2+1 && (x==w/2 || x==w/2+1))){
        return 0;
      }
    }
    return 1;
  }

  int* check_st(int x, int y){
    int hm = grid[y*h+x];
    int temp[4] = {1,1,1,1};
    std::copy(temp, temp+4, this->lc);
    int hl = 0;
    int hr = 0;
    int vd = 0;
    int vu = 0;

    // Horizontal lc[0]
    if(x < 4){ hl = x; }
    else{ hl = 4; }
    for(int i = 1; i<=hl; i++){
      if(grid[y*h+(x-i)] == hm){ lc[0]++; }
      else{ break; }
    }

    if(x > w-4-1){ hr = w-x-1; }
    else{ hr = 4; }
    for(int i = 1; i<=hr; i++){
      if(grid[y*h+(x+i)] == hm){ lc[0]++; }
      else{ break; }
    }

    // Vertical lc[1]
    if(y < 4){ vd = y; }
    else{ vd = 4; }
    for(int i = 1; i<=vd; i++){
      if(grid[(y-i)*h+x] == hm){ lc[1]++; }
      else{ break; }
    }

    if(y > h-4-1){ vu = h-y-1; }
    else{ vu = 4; }
    for(int i = 1; i<=vu; i++){
      if(grid[(y+i)*h+x] == hm){ lc[1]++; }
      else{ break; }
    }

    // Diagonal Right Up lc[2]
    for(int i = 1; i<=(hr<vu?hr:vu); i++){
      if(grid[(y+i)*h+(x+i)] == hm){ lc[2]++; }
      else{ break; }
    }
    for(int i = 1; i<=(hl<vd?hl:vd); i++){
      if(grid[(y-i)*h+(x-i)] == hm){ lc[2]++; }
      else{ break; }
    }

    // Diagonal Left Up lc[3]
    for(int i = 1; i<=(hl<vu?hl:vu); i++){
      if(grid[(y+i)*h+(x-i)] == hm){ lc[3]++; }
      else{ break; }
    }
    for(int i = 1; i<=(hr<vd?hr:vd); i++){
      if(grid[(y-i)*h+(x+i)] == hm){ lc[3]++; }
      else{ break; }
    }

    return lc;
  }

  // Check if there are any connect-5 in all directions of this position
  // Ignore if the home position is empty
  int check(int x, int y){
    if(grid[y*h+x] == MT){
      return 0;
    }
    this->check_st(x, y);
    for(int i = 0; i < 4; i++){
      if(lc[i]==5){
        return grid[y*h+x];
      }
    }
    return 0;
  }

  float pos_eval(int x, int y){
    if(x<0 || y<0 || x>14 || y>14){
      return 0;
    }
    if(grid[y*h+x] == MT){
      return 0;
    }
    this->check_st(x, y);
    float temp = 0;
    for(int i = 0; i < 4; i++){
      temp += std::pow(lc[i], 2);
    }
    temp/=100;
    return temp;
  }

  // Return the symbol respective to the position of the board
  char color(int x, int y){
    if(grid[y*h+x] == BK){
      return BK_char;
    }
    if(grid[y*h+x] == WH){
      return WH_char;
    }
    if(place_perm(x,y)){return MT_char;}
    return NT_char;
  }

  void print(){
    for(int i = h-1; i>=0; i--){
      std::cout << ' ' << i%10;
      for(int j = 0; j<w; j++){
        std::cout << ' ' << this->color(j, i);
      }
      std::cout << std::endl;
    }
    std::cout << "  ";
    for(int j = 0; j<w; j++){
      std::cout << ' ' << j%10;
    }
    std::cout << std::endl;
  }

  void print_marked(int x, int y){
    for(int i = h-1; i>=0; i--){
      std::cout << ' ' << i%10;
      for(int j = 0; j<w; j++){
        if(j==x && i==y){
          std::cout << '<' << this->color(j, i);
        }
        else{
          if(j==x+1 && i==y){
            std::cout << '>' << this->color(j, i);
          }
          else{
            std::cout << ' ' << this->color(j, i);
          }
        }
      }
      if(x==w-1 && i==y){
        std::cout << '>';
      }
      std::cout << std::endl;
    }
    std::cout << "  ";
    for(int j = 0; j<w; j++){
      std::cout << ' ' << j%10;
    }
    std::cout << std::endl;
  }
};


#endif // gomoku_game
