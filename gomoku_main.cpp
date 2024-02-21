#include "gomoku_game.cpp"
#include "gomoku_mnmx.cpp"
#include <fstream>
#include <chrono>

std::chrono::system_clock ze_warudo;
std::chrono::time_point<std::chrono::system_clock> start;
std::chrono::time_point<std::chrono::system_clock> checkpoint;

board main_board;
int mx = -1;
int my = -1;
node mt_node;

void start_timer(){
  start = ze_warudo.now();
}

int stop_timer(){
  checkpoint = ze_warudo.now();
  int dt = std::chrono::duration_cast<std::chrono::milliseconds>(checkpoint - start).count();
  std::cout << "The previous function took : " << dt << " ms" << std::endl;
  return dt;
}

int wh_turn(){
  std::cout << "White's Turn: ";
  /*
  std::cin >> mx >> my;
  std::cout << std::endl;
  */
  std::cout << std::endl;

  if(main_board.turn==0){
    std::cout << 7 << ' ' << 7 << std::endl;
    return main_board.place(7, 7, WH);
  }

  start_timer();

  board tmp_board;
  tmp_board.turn = main_board.turn;
  std::copy(main_board.grid, main_board.grid+225, tmp_board.grid);
  std::copy(main_board.val, main_board.val+225, tmp_board.val);
  mt_node.x = mx;
  mt_node.y = my;
  mt_node = maximizer(mt_node, 4, 0, WH, tmp_board, std::numeric_limits<float>::max());
  mx = mt_node.x;
  my = mt_node.y;
  std::cout << mx << ' ' << my << std::endl;

  stop_timer();

  return main_board.place(mx, my, WH);
}

int bk_turn(){
  std::cout << "Black's Turn: ";
  /*
  std::cin >> mx >> my;
  std::cout << std::endl;
  */
  std::cout << std::endl;

  start_timer();

  board tmp_board;
  tmp_board.turn = main_board.turn;
  std::copy(main_board.grid, main_board.grid+225, tmp_board.grid);
  std::copy(main_board.val, main_board.val+225, tmp_board.val);
  mt_node.x = mx;
  mt_node.y = my;
  mt_node = s_maximizer(mt_node, 4, 0, BK, tmp_board, std::numeric_limits<float>::max());
  mx = mt_node.x;
  my = mt_node.y;
  std::cout << mx << ' ' << my << std::endl;

  stop_timer();

  return main_board.place(mx, my, BK);
}

int main(){
  int state = 1;

  while(state){
    main_board.print_marked(mx, my);
    std::cout << "Turn " << main_board.turn << std::endl;
    if(main_board.turn%2==0){
      while(wh_turn()){}
    }
    else{
      while(bk_turn()){}
    }
    if(main_board.check(mx, my)){
      state = 0;
    }
  }

  main_board.print_marked(mx, my);
  if(main_board.check(mx, my)==WH){
    std::cout << "White won!";
  }
  if(main_board.check(mx, my)==BK){
    std::cout << "Black won!";
  }
  if(main_board.turn == 225){
    std::cout << "Draw!";
  }

  return 0;
}
