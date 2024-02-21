#include <limits>
#include <cmath>
#include "gomoku_game.cpp"

class node{
public:
  int x;
  int y;
  float z;

  node(int x, int y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
  node(){}
  float eval(){
    return z*25+((float)y)/1.4+((float)x)/14;
  }
};

class node_list{
public:
  int total = 0;
  int length = 8;
  node* evythg;

  node_list(){
    evythg = new node[length];
  }
  void purge(){
    delete[] evythg;
  }
  void expand(){
    length *= 2;
    node* temp = new node[length];
    std::copy(evythg, evythg+length/2, temp);
    delete[] evythg;
    evythg = temp;
  }
  int insert(node a){
    /*
    std::cout << total << std::endl;
    for(int j = 0; j<total; j++){
      std::cout << evythg[j].z << " ";
    }
    std::cout << std::endl;
    */
    if(total >= length){
      this->expand();
    }
    if(total == 0){
      evythg[0] = a;
      total++;
      return 0;
    }
    int i = 0;
    while(i < total){
      if(a.eval()>evythg[i].eval()){
        node* temp = new node[total-i];
        std::copy(evythg+i, evythg+total, temp);
        evythg[i] = a;
        std::copy(temp, temp+total-i, evythg+i+1);
        delete[] temp;
        break;
      }
      i++;
    }
    if(i == total){
      evythg[i] = a;
    }
    total++;
  }
};

node minimizer(node me, int t_depth, int depth, int player, board cpy_board, float left_max);
node maximizer(node me, int t_depth, int depth, int player, board cpy_board, float left_min);

node minimizer(node me, int t_depth, int depth, int player, board cpy_board, float left_max){
  if(cpy_board.check(me.x, me.y)!=MT){
    me.z = std::numeric_limits<float>::max();
    return me;
  }

  // Returning end nodes
  if(t_depth == depth){
    float total = 0;
    for(int i = 0; i < cpy_board.h; i++){
      for(int j = 0; j < cpy_board.w; j++){
        if(cpy_board.place_perm(j,i)){
          total += cpy_board.get_val(j-1,i, player)+cpy_board.get_val(j+1,i, player)+cpy_board.get_val(j,i-1, player)+cpy_board.get_val(j,i+1, player)+
                    cpy_board.get_val(j-1,i-1, player)+cpy_board.get_val(j+1,i-1, player)+cpy_board.get_val(j-1,i+1, player)+cpy_board.get_val(j+1,i+1, player);
        }
        total += cpy_board.get_val(j, i, player)*10;
      }
    }
    me.z = total;
    return me;
  }

  // Creating daughter nodes
  node_list node_to_check;
  for(int i = 0; i < cpy_board.h; i++){
    for(int j = 0; j < cpy_board.w; j++){
      if(cpy_board.place_perm(j,i)){
        float temp_total = cpy_board.get_val(j-1,i, MT)+cpy_board.get_val(j+1,i, MT)+cpy_board.get_val(j,i-1, MT)+cpy_board.get_val(j,i+1, MT)+
                            cpy_board.get_val(j-1,i-1, MT)+cpy_board.get_val(j+1,i-1, MT)+cpy_board.get_val(j-1,i+1, MT)+cpy_board.get_val(j+1,i+1, MT);
        temp_total = -temp_total;
        if(temp_total>0.05){
          node temp(j, i, temp_total);
          node_to_check.insert(temp);
        }
      }
      else{
        node temp(j, i, -(std::pow(cpy_board.center_x-j,2)+std::pow(cpy_board.center_y-i,2)));
        node_to_check.insert(temp);
      }
    }
  }

  // Evaluating daughter nodes
  node min_node(-1, -1, std::numeric_limits<float>::max());
  for(int i = node_to_check.total-1; i >= 0; i--){
    board tmp_board;
    tmp_board.turn = cpy_board.turn;
    tmp_board.center_x = cpy_board.center_x;
    tmp_board.center_y = cpy_board.center_y;
    std::copy(cpy_board.grid, cpy_board.grid+225, tmp_board.grid);
    std::copy(cpy_board.val, cpy_board.val+225, tmp_board.val);
    node tmp_node;
    tmp_node = node_to_check.evythg[i];
    tmp_board.place(tmp_node.x, tmp_node.y, (player==WH)?BK:WH);

    node result_node = maximizer(tmp_node, t_depth, depth+1, player, tmp_board, min_node.z);
    tmp_node.z = result_node.z;

    tmp_board.purge();

    if(tmp_node.z < min_node.z || min_node.x == -1){
      min_node = tmp_node;
    }
    // Alpha-Beta Pruning

    if(tmp_node.z <= left_max){
      node_to_check.purge();
      cpy_board.purge();
      return tmp_node;
    }
  }
  node_to_check.purge();
  cpy_board.purge();
  return min_node;
}

node maximizer(node me, int t_depth, int depth, int player, board cpy_board, float left_min){
  if(cpy_board.check(me.x, me.y)!=MT){
    me.z = -std::numeric_limits<float>::max();
    return me;
  }

  // Returning end nodes
  if(t_depth == depth){
    float total = 0;
    for(int i = 0; i < cpy_board.h; i++){
      for(int j = 0; j < cpy_board.w; j++){
        if(cpy_board.place_perm(j,i)){
          total += cpy_board.get_val(j-1,i, player)+cpy_board.get_val(j+1,i, player)+cpy_board.get_val(j,i-1, player)+cpy_board.get_val(j,i+1, player)+
                    cpy_board.get_val(j-1,i-1, player)+cpy_board.get_val(j+1,i-1, player)+cpy_board.get_val(j-1,i+1, player)+cpy_board.get_val(j+1,i+1, player);
        }
        total += cpy_board.get_val(j, i, player)*10;
      }
    }
    me.z = total;
    return me;
  }

  // Creating daughter nodes
  node_list node_to_check;
  for(int i = 0; i < cpy_board.h; i++){
    for(int j = 0; j < cpy_board.w; j++){
      if(cpy_board.place_perm(j,i)){
        float temp_total = cpy_board.get_val(j-1,i, MT)+cpy_board.get_val(j+1,i, MT)+cpy_board.get_val(j,i-1, MT)+cpy_board.get_val(j,i+1, MT)+
                            cpy_board.get_val(j-1,i-1, MT)+cpy_board.get_val(j+1,i-1, MT)+cpy_board.get_val(j-1,i+1, MT)+cpy_board.get_val(j+1,i+1, MT);
        temp_total = -temp_total;
        if(temp_total>0.05){
          node temp(j, i, temp_total);
          node_to_check.insert(temp);
        }
      }
      else{
        node temp(j, i, -(std::pow(cpy_board.center_x-j,2)+std::pow(cpy_board.center_y-i,2)));
        node_to_check.insert(temp);
      }
    }
  }

  // Evaluating daughter nodes
  node max_node(-1, -1, -std::numeric_limits<float>::max());
  for(int i = 0; i < node_to_check.total; i++){
    board tmp_board;
    tmp_board.turn = cpy_board.turn;
    tmp_board.center_x = cpy_board.center_x;
    tmp_board.center_y = cpy_board.center_y;
    std::copy(cpy_board.grid, cpy_board.grid+225, tmp_board.grid);
    std::copy(cpy_board.val, cpy_board.val+225, tmp_board.val);
    node tmp_node;
    tmp_node = node_to_check.evythg[i];
    tmp_board.place(tmp_node.x, tmp_node.y, (player==WH)?WH:BK);

    node result_node = minimizer(tmp_node, t_depth, depth+1, player, tmp_board, max_node.z);
    tmp_node.z = result_node.z;

    tmp_board.purge();

    if(tmp_node.z > max_node.z || max_node.x == -1){
      max_node = tmp_node;
    }
    // Alpha-Beta Pruning

    if(tmp_node.z >= left_min){
      node_to_check.purge();
      cpy_board.purge();
      return tmp_node;
    }

  }
  node_to_check.purge();
  cpy_board.purge();
  return max_node;
}

node s_minimizer(node me, int t_depth, int depth, int player, board cpy_board, float left_max);
node s_maximizer(node me, int t_depth, int depth, int player, board cpy_board, float left_min);

node s_minimizer(node me, int t_depth, int depth, int player, board cpy_board, float left_max){
  if(cpy_board.check(me.x, me.y)!=MT){
    me.z = std::numeric_limits<float>::max();
    return me;
  }

  // Returning end nodes
  if(t_depth == depth){
    float total = 0;
    for(int i = 0; i < cpy_board.h; i++){
      for(int j = 0; j < cpy_board.w; j++){
        if(cpy_board.place_perm(j,i)){
          total += cpy_board.get_val(j-1,i, player)+cpy_board.get_val(j+1,i, player)+cpy_board.get_val(j,i-1, player)+cpy_board.get_val(j,i+1, player)+
                    cpy_board.get_val(j-1,i-1, player)+cpy_board.get_val(j+1,i-1, player)+cpy_board.get_val(j-1,i+1, player)+cpy_board.get_val(j+1,i+1, player);
        }
        total += cpy_board.get_val(j, i, player)*10;
      }
    }
    me.z = total;
    return me;
  }

  // Creating daughter nodes
  node_list node_to_check;
  for(int i = 0; i < cpy_board.h; i++){
    for(int j = 0; j < cpy_board.w; j++){
      if(cpy_board.place_perm(j,i)){
        float temp_total = cpy_board.get_val(j-1,i, MT)+cpy_board.get_val(j+1,i, MT)+cpy_board.get_val(j,i-1, MT)+cpy_board.get_val(j,i+1, MT)+
                            cpy_board.get_val(j-1,i-1, MT)+cpy_board.get_val(j+1,i-1, MT)+cpy_board.get_val(j-1,i+1, MT)+cpy_board.get_val(j+1,i+1, MT);
        temp_total = -temp_total;
        if(temp_total>0.05){
          node temp(j, i, temp_total);
          node_to_check.insert(temp);
        }
      }
    }
  }

  // Evaluating daughter nodes
  node min_node(-1, -1, std::numeric_limits<float>::max());
  for(int i = node_to_check.total-1; i >= 0; i--){
    board tmp_board;
    tmp_board.turn = cpy_board.turn;
    tmp_board.center_x = cpy_board.center_x;
    tmp_board.center_y = cpy_board.center_y;
    std::copy(cpy_board.grid, cpy_board.grid+225, tmp_board.grid);
    std::copy(cpy_board.val, cpy_board.val+225, tmp_board.val);
    node tmp_node;
    tmp_node = node_to_check.evythg[i];
    tmp_board.place(tmp_node.x, tmp_node.y, (player==WH)?BK:WH);

    node result_node = s_maximizer(tmp_node, t_depth, depth+1, player, tmp_board, min_node.z);
    tmp_node.z = result_node.z;

    tmp_board.purge();

    if(tmp_node.z < min_node.z || min_node.x == -1){
      min_node = tmp_node;
    }
    // Alpha-Beta Pruning

    if(tmp_node.z <= left_max){
      node_to_check.purge();
      cpy_board.purge();
      return tmp_node;
    }
  }
  node_to_check.purge();
  cpy_board.purge();
  return min_node;
}

node s_maximizer(node me, int t_depth, int depth, int player, board cpy_board, float left_min){
  if(cpy_board.check(me.x, me.y)!=MT){
    me.z = -std::numeric_limits<float>::max();
    return me;
  }

  // Returning end nodes
  if(t_depth == depth){
    float total = 0;
    for(int i = 0; i < cpy_board.h; i++){
      for(int j = 0; j < cpy_board.w; j++){
        if(cpy_board.place_perm(j,i)){
          total += cpy_board.get_val(j-1,i, player)+cpy_board.get_val(j+1,i, player)+cpy_board.get_val(j,i-1, player)+cpy_board.get_val(j,i+1, player)+
                    cpy_board.get_val(j-1,i-1, player)+cpy_board.get_val(j+1,i-1, player)+cpy_board.get_val(j-1,i+1, player)+cpy_board.get_val(j+1,i+1, player);
        }
        total += cpy_board.get_val(j, i, player)*10;
      }
    }
    me.z = total;
    return me;
  }

  // Creating daughter nodes
  node_list node_to_check;
  for(int i = 0; i < cpy_board.h; i++){
    for(int j = 0; j < cpy_board.w; j++){
      if(cpy_board.place_perm(j,i)){
        float temp_total = cpy_board.get_val(j-1,i, MT)+cpy_board.get_val(j+1,i, MT)+cpy_board.get_val(j,i-1, MT)+cpy_board.get_val(j,i+1, MT)+
                            cpy_board.get_val(j-1,i-1, MT)+cpy_board.get_val(j+1,i-1, MT)+cpy_board.get_val(j-1,i+1, MT)+cpy_board.get_val(j+1,i+1, MT);
        temp_total = -temp_total;
        if(temp_total>0.05){
          node temp(j, i, temp_total);
          node_to_check.insert(temp);
        }
      }
    }
  }

  // Evaluating daughter nodes
  node max_node(-1, -1, -std::numeric_limits<float>::max());
  for(int i = 0; i < node_to_check.total; i++){
    board tmp_board;
    tmp_board.turn = cpy_board.turn;
    tmp_board.center_x = cpy_board.center_x;
    tmp_board.center_y = cpy_board.center_y;
    std::copy(cpy_board.grid, cpy_board.grid+225, tmp_board.grid);
    std::copy(cpy_board.val, cpy_board.val+225, tmp_board.val);
    node tmp_node;
    tmp_node = node_to_check.evythg[i];
    tmp_board.place(tmp_node.x, tmp_node.y, (player==WH)?WH:BK);

    node result_node = s_minimizer(tmp_node, t_depth, depth+1, player, tmp_board, max_node.z);
    tmp_node.z = result_node.z;

    tmp_board.purge();

    if(tmp_node.z > max_node.z || max_node.x == -1){
      max_node = tmp_node;
    }
    // Alpha-Beta Pruning

    if(tmp_node.z >= left_min){
      node_to_check.purge();
      cpy_board.purge();
      return tmp_node;
    }

  }
  node_to_check.purge();
  cpy_board.purge();
  return max_node;
}
