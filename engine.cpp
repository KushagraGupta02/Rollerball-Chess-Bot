#include <algorithm>
#include <random>
#include <iostream>
#include "bits/stdc++.h"
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"


using namespace std;

// define start time 
auto st = std::chrono::high_resolution_clock::now();
double time_for_move;


// atomic<bool>* flag;

const float pawn_mtx[7][7] = {  {1.40,1.7,2.2,2.8,1.0,0.9,0.9},
                                {1.40,1.8,2.3,3.0,1.0,1.1,1.05},
                                {1.35,1.4,0.0,0.0,0.0,1.2,1.15},
                                {1.25,1.3,0.0,0.0,0.0,1.3,1.25},
                                {1.15,1.2,0.0,0.0,0.0,1.4,1.35},
                                {1.05,1.1,1.0,3.0,2.3,1.8,1.40},
                                {0.9,0.9,1.0,2.8,2.2,1.7,1.40}                                
};

const float pawn_mtx_2_white[8][8] = {
    {1.65,1.85,2.2,2.5,2.8,4.0,0.0,0.0},
    {1.70,1.90,2.3,2.6,2.9,4.0,0.0,0.0},
    {1.55,1.60,0.0,0.0,0.0,0.0,0.0,0.0},
    {1.40,1.45,0.0,0.0,0.0,0.0,0.0,0.0},
    {1.30,1.35,0.0,0.0,0.0,0.0,0.0,0.0},
    {1.20,1.25,0.0,0.0,0.0,0.0,0.0,0.0},
    {1.10,1.15,1.1,1.0,0.95,0.9,0.0,0.0},
    {1.0,1.1,1.05,0.95,0.90,0.85,0.0,0.0}
};
const float pawn_mtx_2_black[8][8] = {
    {0.0,0.0,0.85,0.90,1.0,1.05,1.1,1.0},
    {0.0,0.0,0.9,0.95,1.05,1.1,1.15,1.1},
    {0.0,0.0,0.0,0.0,0.0,0.0,1.25,1.20},
    {0.0,0.0,0.0,0.0,0.0,0.0,1.35,1.30},
    {0.0,0.0,0.0,0.0,0.0,0.0,1.45,1.40},
    {0.0,0.0,0.0,0.0,0.0,0.0,1.60,1.55},
    {0.0,0.0,4.0,2.9,2.6,2.3,1.90,1.70},
    {0.0,0.0,4.0,2.8,2.5,2.2,1.85,1.65}
};


// To be changed later like pawn_mtx_2
const float pawn_mtx_3_white[8][8] = {
    {1.3,1.6,2.0,2.4,2.8,4.0,0.0,0.0},
    {1.3,1.5,2.0,2.5,2.9,4.0,0.0,0.0},
    {1.3,1.5,2.2,2.5,2.9,4.0,0.0,0.0},
    {1.2,1.25,1.8,0.0,0.0,0.0,0.0,0.0},
    {1.15,1.2,1.5,0.0,0.0,0.0,0.0,0.0},
    {1.05,1.1,1.3,1.0,0.9,0.9,0.0,0.0},
    {1.05,1.05,1.0,1.0,0.9,0.0,0.0,0.0},
    {1.0,1.0,0.9,0.8,0.9,0.0,0.0,0.0}
};

const float pawn_mtx_3_black[8][8] = {
    {0.0,0.0,0.0,0.9,0.9,0.8,1.0,1.0},
    {0.0,0.0,0.0,0.9,1.0,1.0,1.05,1.05},
    {0.0,0.0,0.9,0.9,1.0,1.3,1.1,1.05},
    {0.0,0.0,0.0,0.0,0.0,1.5,1.2,1.15},
    {0.0,0.0,0.0,0.0,0.0,1.8,1.25,1.2},
    {0.0,0.0,4.0,2.9,2.5,2.2,1.5,1.3},
    {0.0,0.0,4.0,2.9,2.5,2.2,1.5,1.3},
    {0.0,0.0,4.0,2.8,2.4,2.0,1.6,1.3}
};

unordered_map<string, string> open_book_seven_white ={{"..rbp..\n.r.k.p.\n..   ..\n.P   ..\n..   ..\n...KR..\n..PBR..\n" , "d1d7"},
{"..rbp..\n..rk.p.\n..   ..\n..   ..\n.P   ..\n...KR..\n..PBR..\n" , "b3b4"},
{"..rbp..\n..rkp..\n..   ..\n..   ..\n..   ..\n..PKR..\n..PBR..\n", "c2b3"},
{"..rkp..\n.r...p.\n..   ..\n.P   ..\n..   ..\n...KR..\n..P.R..\n" , "c1b2"},
{"..rkp..\n.r.....\n..   p.\n.P   ..\n..   ..\n.P.KR..\n....R..\n" , "e1a5"},
{"..rkp..\n......r\nR.   p.\n.P   ..\n..   ..\n.P.KR..\n.......\n" , "a5a6"},
{ "..rkp..\nR......\n..   p.\n.P   ..\n..   ..\n.P.KR..\n.....r.\n","e2e1"},
{"..rkp..\nR......\n..   p.\n.P   ..\n..   ..\n.P.K.r.\n....R..\n" ,"d2c1"},
{"..rkp..\nR......\n..   p.\n.P   ..\n..   ..\n.P....r\n..K.R..\n" ,"e1d1"}};


unordered_map<string, string> open_book_seven_black ={
{"..rbp..\n..rkp..\n..   ..\n..   ..\n..   ..\n..PK.R.\n..PBR..\n" , "e6f5"},
{"..rbp..\n..rkp..\n..   ..\n..   ..\n.P   ..\n...KR..\n..PBR..\n" , "e6f5"},
{"..rbp..\n..rk...\n..   p.\n..   ..\n..   ..\n..PKBR.\n..P.R..\n" , "f5f4"},
{"..rbp..\n..rk...\n..   ..\n..   p.\n..   ..\n.PPKBR.\n....R..\n", "d7e2"},
{"..rbp..\n..rkp..\n..   ..\n..   ..\n..   ..\n.P.KR..\n..PBR..\n" , "c6b6"},
{"..rbp..\n.r.kp..\n..   ..\n..   ..\n..   ..\n.P.K.R.\n..PBR..\n" , "e6f5"},
{"..rbp..\n.r.k...\n..   p.\n..   ..\nP.   ..\n...K.R.\n..PBR..\n" , "f5f4"},
{ "..rbp..\n.r.k...\n..   ..\n..   p.\nP.   ..\n...K...\n..PBRR.\n","d7d1"},
{"..r.p..\n.r.k...\n..   ..\n..   p.\nP.   ..\n.......\n..PKRR.\n" ,"f4f3"},
{"..r.p..\n.r.k...\n..   ..\n..   ..\nP.   p.\n.P.....\n...KRR.\n" ,"e7f6"},
{"..r....\n.r.k.p.\n..   ..\n..   ..\nPP   p.\n.......\n...KRR.\n", "c7g2"},
{".......\n.r.k.p.\n..   ..\n.P   ..\n.P   p.\n......r\n...KRR.\n","b6b7"},
{"..rbp..\n..rk...\n..   p.\n.P   ..\n..   ..\n...KR..\n..PBR..\n" , "e6f5"},
{"..rbp..\n..rk...\n.P   ..\n..   .p\n..   ..\n...KR..\n..PBR..\n" , "f5g4"},
{"..rbp..\n..rk...\n..   p.\n.P   ..\n..   ..\n...KR..\n..PBR..\n", "f4f3"},
{"..rbp..\n..rk...\n.P   ..\n..   p.\n..   ..\n...KR..\n..PBR..\n" , "d7c6"},
{"..rbp..\n..Pk...\n..   ..\n..   ..\n..   p.\n...KR..\n..PBR..\n" , "e7f6"},
{"..r.p..\n..bk...\n..   ..\n..   ..\n..   p.\n...KR..\n..PB.R.\n" , "d6c6"},
{ "..r....\n..Bk.p.\n..   ..\n..   ..\n..   p.\n...KR..\n..P..R.\n","c7g2"},
{"..r....\n..k..p.\n..   ..\n..   ..\n..   p.\n...K...\n..P.RR.\n" ,"f3f2"},
{".......\n..k..p.\n..   ..\n..   ..\n..   p.\n......r\n..PKRR.\n" ,"g2f2"},
{".......\n..k..p.\n..   ..\n..   ..\n..   ..\n.....Rr\n..PKR..\n", "f2g2"},
{".......\n..k..p.\n..   ..\n..   ..\n..   ..\n....Rr.\n..PK...\n","f6f5"}


};

unordered_map<string, string> open_book_eightfour_black ={
{"..prbp..\n..prkp..\n..    ..\n..    ..\n..    ..\n.P    ..\n...KRP..\n..PBRP..\n" , "f7g7"},
{"..prbp..\n..prk.p.\n..    ..\n..    ..\n.P    ..\n..    ..\n...KRP..\n..PBRP..\n" , "e8h5"},
{"..pr.p..\n..prk.p.\n..    ..\n..    .b\n.P    ..\n..    ..\n...KRP..\n.P.BRP..\n", "d8e8"},
{"..p.rp..\n..prk.p.\n..    ..\n.P    .b\n..    ..\n..    ..\n...KRP..\n.P.BRP..\n" , "g7h6"},
{"..p.rp..\n..prk...\n..    .p\n.P    .b\n..    ..\n..    ..\n....RP..\n.PKBRP..\n" , "e7d8"},
{"..pkrp..\n..pr....\n..    .p\n.P    .b\n..    ..\n..    ..\nR....P..\n.PKBRP..\n" , "d7g7"},
{ "..pkrp..\n..p...r.\n.P    .p\n..    .b\n..    ..\n..    ..\nR....P..\n.PKBRP..\n","f8g8"},
{ "..pkr.p.\n.Pp...r.\n..    .p\n..    .b\n..    ..\n..    ..\nR....P..\n.PKBRP..\n" ,"d8e7"},
{"..p.r.p.\n..P.k.r.\n..    .p\n..    .b\n..    ..\n..    ..\nR....P..\n.PKBRP..\n" ,"g7g2"},
{"..R.r.p.\n..P.k...\n..    .p\n..    .b\n..    ..\n..    ..\n.....Pr.\n.PKBRP..\n", "g8h7"},
{"..R.r...\n..P.k..p\n..    .p\n..    .b\n..    ..\n..    ..\n....PPr.\n.PKBR...\n","g2h2"},
{"..R.r...\n..P.k..p\n..    .p\n..    .b\n..    ..\n..    ..\nP...PP.r\n..KBR...\n" , "h5g6"},
{"..R.r...\n..P.k..p\n..    bp\n..    ..\n..    ..\n..    ..\nPK..PP.r\n...BR...\n", "h2g1"},
{"..R.r...\n..P.k..p\n..    bp\n..    ..\n..    ..\nP.    ..\n.K..PP..\n...BR.r.\n","h6g5"},
{"....R...\n..P.k..p\n..    b.\n..    p.\n..    ..\nP.    ..\n.K..PP..\n...BR.r.\n","e7e8"}};

unordered_map<string, string> open_book_eightfour_white ={
{"..prbp..\n..prkp..\n..    ..\n..    ..\n..    ..\n..    ..\n..PKRP..\n..PBRP..\n","c2b3"},
{"..prbpp.\n..prk...\n..    ..\n..    ..\n..    ..\n.P    ..\n...KRP..\n..PBRP..\n","c2b3"},
{"..pr.pp.\n..prk...\n..    ..\n..    ..\n..    b.\n.P    ..\n..BKRP..\n..P.RP..\n","d1c2"},
{"..pr.p..\n..prk..p\n..    ..\n..    ..\n..    b.\n.P    ..\n.PBKRP..\n....RP..\n","c1b2"},
{"..pr.p..\n..pr.k.p\n..    ..\n..    ..\n.P    b.\n..    ..\n.PBKRP..\n....RP..\n","b3b4"},
{"..pr.p..\n..pr.k.p\nR.    ..\n..    ..\n.P    ..\n..    ..\n.PBKRP..\n.....b..\n","e1a6"},
{"..pr.p..\n..pr.k.p\nR.    ..\n..    ..\n.P    ..\n..    ..\n.PBK.Pb.\n....R...\n","e2e1"},
{"..pr.p..\n..p.rk.p\nR.    ..\n..    ..\n.P    ..\n..    ..\n.PBK.Pb.\nR.......\n","e1a1"},
{"..r..p..\n..p.rk.p\n..    ..\n..    ..\n.P    ..\n..    ..\n.PBK.Pb.\nR.......\n","a6c8"},
{"..R..p..\n...prk.p\n..    ..\n..    ..\n.P    ..\n..    ..\n.PBK.Pb.\n........\n","a1c8"},
{"..R.rp..\n...p.k.p\n..    ..\n.P    ..\n..    ..\n..    ..\n.PBK.Pb.\n........\n","b4b5"},
{"..R.r...\n...p.kpp\n.P    ..\n..    ..\n..    ..\n..    ..\n.PBK.Pb.\n........\n","b5b6"},
{"..R.r...\n..Pp.kp.\n..    p.\n..    ..\n..    ..\n..    ..\n.PBK.Pb.\n........\n","b6c7"},
{"..R.....\n...P.kp.\n..    p.\n..    ..\n..    ..\n..    .r\n.PBK.Pb.\n........\n","c7d7"},
{"..R.P...\n....k.p.\n..    p.\n..    ..\n..    ..\n..    .r\n.PBK.Pb.\n........\n","d7e8"},
{"..R.P...\n....k.p.\n..    p.\n..    ..\n..    ..\n..    ..\n.PBK..br\n....P...\n","f2e1"},
{"..R.....\n.....kp.\n..    p.\n..    ..\n..    ..\n..    ..\n.PBK..br\n....P...\n","e8f7r"}};

unordered_map<string, string> open_book_eighttwo_white ={
{"..r..p..\n..rknp..\n..pbnp..\n...  ...\n...  ...\n..PNBP..\n..PNKR..\n..P..R..\n","d2c4"},
{"..r.kp..\n..r.np..\n..pbnp..\n...  ...\n..N  ...\n..PNBP..\n..P.KR..\n..P..R..\n","c4d6"},
{"..r.kp..\n..r.np..\n...pnp..\n...  ...\n...  ...\n..PNBP..\n..P.KR..\n..P..R..\n","e2e1"},
{"..r.kp..\n..r..p..\n...pnp..\n...  n..\n...  ...\n..PN.P..\n..PBKR..\n..P..R..\n","f1g1"},
{"..r.kp..\n..r.....\n...pnpp.\n...  n..\n...  ...\n..PN.P..\n..PBK.R.\n..P..R..\n","d3b2"},
{"..r.kp..\n.r......\n...pnpp.\n...  n..\n...  ...\n..PN.P..\n.PPBK.R.\n.....R..\n","d2c4"}
};
unordered_map<string, string> open_book_eighttwo_black ={
    {"..r..p..\n..rknp..\n..pbnp..\n...  ...\n...  ...\n..PNBP..\n.PPNKR..\n.....R..\n","e7f5"},
    {"..r..p..\n..rk.p..\n..pbnp..\n...  n..\n...  ...\n.PPNBP..\n..PNKR..\n.....R..\n","f5g3"},
    {"..r..p..\n..rk.p..\n..pbnp..\n...  ...\n...  ...\n.PPNBP..\n..PN....\n...K.R..\n","g3f1"},
    {"..rk.p..\n..r..p..\n..pbnp..\n...  ...\n..P  ...\n..PNBP..\n..PN....\n...K.R..\n","f8g8"},
    {"..rk..p.\n..r..p..\n..pbnp..\n...  ...\n..P  N..\n..P.BP..\n..PN....\n...K.R..\n","e6f4"},
    {"..rk..p.\n..r..p..\n..pb.p..\n...  ...\n..P  B..\n..P..P..\n..PN....\n...K.R..\n","f7g6"}
    };


unordered_map<string, int> tt;
unordered_map<string,pair<int,int>> table_for_max, table_for_min;
int max_tt_size = 10000000;

int countAttackingPairs(const Board& b, const std::unordered_set<U16>& moves, const string &s) {
    int attackingPairs = 0;
    // Iterate through the moves
    for (const U16 move : moves) {
        U8 to = getp1(move);  
        // cout<<"column"<<(to/8) <<" row"<< to%8 <<"\n";
        if ((b.data.board_0[to] & 0x1F) != 0) { //00011111
            // Check if the captured piece belongs to the opponent
            if (b.data.board_0[to] & (b.data.player_to_play ^ (WHITE | BLACK))) {
                // Increment the attackingPairs count
                attackingPairs++;
            }
        }
    }
    return attackingPairs;
}

int Hanging(const Board& b, const std::unordered_set<U16>& moves, const string &s) {
    set<U8> hang;
    for(int i = 0;i<64;i++)
    {
        if ((b.data.board_0[i] & 0x1F) != 0) {
            if (b.data.board_0[i] & (b.data.player_to_play)) {
                hang.insert(b.data.board_0[i]);
            }
        }
    }
    for (const U16 move : moves) {
        U8 to = getp1(move);  
        if ((b.data.board_0[to] & 0x1F) != 0) {
            if (b.data.board_0[to] & (b.data.player_to_play)) {
                hang.erase(b.data.board_0[to]);
            }
        }
    }
    double def = 0;
    for(auto piece: hang){
        if      (piece & PAWN)   def -= 1;
        else if (piece & ROOK)   def -= 5;
        else if (piece & KNIGHT) def -= 3;
        else if (piece & BISHOP){
            if (b.data.board_type == SEVEN_THREE) def -= 3;
            if (b.data.board_type == EIGHT_TWO) def -= 4;
            else def -= 2;
        }
    }
    return (int)def*3.0;
}



int evaluate_board1(const Board& b) {
    string s = board_to_str(&(b.data));
    if (tt[s]) return tt[s];
    auto x = b.get_legal_moves();
    // // code for attacking pairs, defence pairs and squares covered
    int sc1 = countAttackingPairs(b,x, s);
    int sc2 = Hanging(b,x, s);
    int sc3 = x.size();
    auto player = b.data.player_to_play;
    auto in_check = b.in_check();
    if (x.empty() && in_check && player == WHITE) return -100000;
    if (x.empty() && in_check && player == BLACK) return 100000;
    if (x.empty()) return 0;
    unordered_map<char,int> piece_val;
    piece_val['.'] = 0;
    piece_val['P'] = 1; piece_val['p'] = -1;
    piece_val['R'] = 6; piece_val['r'] = -6;
    piece_val['B'] = 3; piece_val['b'] = -3;
    piece_val['K'] = 1000; piece_val['k'] = -1000;
    double score = 0.0;
    for(int i = 0;i<64;i++){
        if (s[i]=='p'){
            score+=(double)pawn_mtx[i/8][i%8]*piece_val[s[i]];
        }
        else if (s[i]=='P'){
            score+=(double)pawn_mtx[i/8][i%8]*piece_val[s[i]];;
        }
        else if ((s[i]=='r' || s[i] == 'R')&& (i%8 == 0 || i%8 == 6 || i/8 == 0 || i/8 == 6)){
            score+=1.1*(double)piece_val[s[i]]; 
        }
        else score+=(double)piece_val[s[i]];
    }
    int par = (player == WHITE) ? 1 : -1;
    double ans = (double)(100.0*(score) + par*(5.0*(sc1 + sc2 + sc3)));
    if ((int)tt.size()>max_tt_size) tt.clear();
    tt[s] = (int)ans;
    return ans;
}

int evaluate_board2(const Board&b){
    string s = board_to_str(&(b.data));
    if (tt[s]) return tt[s];
    auto x = b.get_legal_moves();
    auto in_check = b.in_check();
    int sc1 = countAttackingPairs(b,x, s);
    int sc2 = Hanging(b,x, s);
    int sc3 = x.size();
    auto player = b.data.player_to_play;
    if (x.empty() && in_check && player == WHITE) return -100000;
    if (x.empty() && in_check && player == BLACK) return 100000;
    if (x.empty()) return 0;
    unordered_map<char,int> piece_val;
    piece_val['.'] = 0;
    piece_val['P'] = 1; piece_val['p'] = -1;
    piece_val['R'] = 6; piece_val['r'] = -6;
    piece_val['B'] = 2; piece_val['b'] = -2;
    piece_val['K'] = 1000; piece_val['k'] = -1000;
    piece_val['N'] = 3; piece_val['n'] = -3;
    double score = 0.0;
    int n = s.size();
    for(int i= 0;i<n;i++){
        if (s[i]=='p'){
            score+=(double)pawn_mtx_2_black[i/9][i%9]*piece_val[s[i]];
        }
        else if (s[i]=='P'){
            score+=(double)pawn_mtx_2_white[i/9][i%9]*piece_val[s[i]];
        }
        else if ((s[i]=='r' || s[i] == 'R')&& (i%9 == 0 || i%9 == 7 || i/9 == 0 || i/9 == 7)){
            score+=1.1*(double)piece_val[s[i]]; 
        }
        else score+=(double)piece_val[s[i]];
    }
    int par = (player == WHITE) ? 1 : -1;
    double ans = (double)(100.0*(score) + par*(5.0*(sc1 + sc2 + sc3)));
    if ((int)tt.size()>max_tt_size) tt.clear();
    tt[s] = (int)ans;
    return ans;
}

int evaluate_board3(const Board&b){
    string s = board_to_str(&(b.data));
    if (tt[s]) return tt[s];
    auto x = b.get_legal_moves();
    auto in_check = b.in_check();
    auto player = b.data.player_to_play;
    int sc1 = countAttackingPairs(b,x, s);
    int sc2 = Hanging(b,x, s);
    int sc3 = x.size();
    if (x.empty() && in_check && player == WHITE) return -100000;
    if (x.empty() && in_check && player == BLACK) return 100000;
    if (x.empty()) return 0;
    unordered_map<char,int> piece_val;
    piece_val['.'] = 0;
    piece_val['P'] = 1; piece_val['p'] = -1;
    piece_val['R'] = 6; piece_val['r'] = -6;
    piece_val['B'] = 4; piece_val['b'] = -4;
    piece_val['K'] = 1000; piece_val['k'] = -1000;
    piece_val['N'] = 3; piece_val['n'] = -3;
    double score = 0.0;
    int n = s.size();
    for(int i= 0;i<n;i++){
        if (s[i]=='p'){
            score+=(double)pawn_mtx_3_black[i/9][i%9]*piece_val[s[i]];
        }
        else if (s[i]=='P'){
            score+=(double)pawn_mtx_3_white[i/9][i%9]*piece_val[s[i]];
        }
        else if ((s[i]=='r' || s[i] == 'R')&& (i%9 == 0 || i%9 == 7 || i/9 == 0 || i/9 == 7)){
            score+=1.1*(double)piece_val[s[i]]; 
        }
        else score+=(double)piece_val[s[i]];
    }
    int par = (player == WHITE) ? 1 : -1;
    double ans = (double)(100.0*(score) + par*(5.0*(sc1 + sc2 + sc3)));
    if ((int)tt.size()>max_tt_size) tt.clear();
    tt[s] = (int)ans;
    return ans;
}







int evaluate_board(const Board& b){
    if (b.data.board_type == SEVEN_THREE){
        return evaluate_board1(b);
    }
    else if (b.data.board_type == EIGHT_FOUR){
        return evaluate_board2(b);
    }
    else if (b.data.board_type == EIGHT_TWO){
        return evaluate_board3(b);
    }
    // raise exception
    return 0;
}





// given a board and a move tell whether this move is a capture or not
bool is_capture(const Board& b, U16 move){
    U8 to = getp1(move);
    if ((b.data.board_0[to] & 0x1F) != 0) { //00011111
        // Check if the captured piece belongs to the opponent
        if (b.data.board_0[to] & (b.data.player_to_play ^ (WHITE | BLACK))) {
            // Increment the attackingPairs count
            return true;
        }
    }
    return false;
}







int minimax(const Board& b, int depth, int alpha, int beta, bool maximizingPlayer,int limit, bool last_move_was_capture = false) {
    auto et = std::chrono::high_resolution_clock::now();
    bool search = (et -st ).count() < time_for_move;
    if (!search) return 0;
    if (limit<= 0){
        return evaluate_board(b);
    }
    if (depth == 0 || b.get_legal_moves().empty()) {
        if (b.in_check()){
           return minimax(b,1,alpha,beta,maximizingPlayer,limit - 1, false);
        }
        if (last_move_was_capture){
            return minimax(b,1,alpha,beta,maximizingPlayer,limit - 1, false);
        }
        int x = evaluate_board(b);
        return x;
    }
    string s = board_to_str(&(b.data));
    if (maximizingPlayer){
        pair<int,int> k = table_for_max[s];
        if (k.first >= depth) {
            return k.second;
        }
    }
    else{
        pair<int,int> k = table_for_min[s];
        if (k.first >= depth) {
            return k.second;
        }
    }
    std::unordered_set<U16> moves = b.get_legal_moves();
    int best_score = maximizingPlayer ? -999999 : 999999;

    for (auto move : moves) {
        Board* nb = new Board(b);
        nb->do_move_(move);
        int score = 0;
        bool flag = is_capture(b,move);
        score = minimax(*nb, depth - 1, alpha, beta, !maximizingPlayer, limit, flag);
        delete nb;
        if (maximizingPlayer) {
            best_score = std::max(best_score, score);
            alpha = std::max(alpha, score);
        } else {
            best_score = std::min(best_score, score);
            beta = std::min(beta, score);
        }

        if (beta <= alpha) {
            break;  // Prune the search
        }

    }
    if (maximizingPlayer) table_for_max[s] = {depth, best_score};
    else table_for_min[s] = {depth, best_score};
    if ((int)table_for_max.size() > max_tt_size) table_for_max.clear();
    if ((int)table_for_min.size() > max_tt_size) table_for_max.clear();
    return best_score;
}

double calc_time(std::chrono::milliseconds time_left){
    double time;
    if (time_left.count() < 10000){
        time = 0.3*time_left.count();
    }
    else if (time_left.count() < 20000){
        time = 0.3*time_left.count();
    }
    else if (time_left.count() < 30000){
        time = 0.3*time_left.count();
    }
    else if (time_left.count() < 40000){
        time = 0.2*time_left.count();
    }
    else if (time_left.count() < 50000){
        time = 0.1*time_left.count();
    }
    else{
        time = 0.07*time_left.count();
    }
    cout<<"Time for move "<<time<<"\n";
    return time*1e6;
}



void Engine::find_best_move(const Board& b) {
    if (b.data.board_type == SEVEN_THREE){
        if (b.data.player_to_play ==WHITE &&  open_book_seven_white.count(board_to_str(&(b.data)))){
            this->best_move = str_to_move(open_book_seven_white[board_to_str(&(b.data))]);
            auto moves = b.get_legal_moves();
            if (moves.count(this->best_move)){return;}

        }
        if (b.data.player_to_play ==BLACK &&  open_book_seven_black.count(board_to_str(&(b.data)))){
            this->best_move = str_to_move(open_book_seven_black[board_to_str(&(b.data))]);
            auto moves = b.get_legal_moves();
            if (moves.count(this->best_move)){return;}
        }
   }
    else if (b.data.board_type == EIGHT_FOUR){
        if (b.data.player_to_play ==WHITE &&  open_book_eightfour_white.count(board_to_str(&(b.data)))){
            this->best_move = str_to_move(open_book_eightfour_white[board_to_str(&(b.data))]);
            auto moves = b.get_legal_moves();
            if (moves.count(this->best_move)){return;}
        }
        if (b.data.player_to_play ==BLACK &&  open_book_eightfour_black.count(board_to_str(&(b.data)))){
            this->best_move = str_to_move(open_book_eightfour_black[board_to_str(&(b.data))]);
            auto moves = b.get_legal_moves();
            if (moves.count(this->best_move)){return;}
        }
    }
    else if (b.data.board_type == EIGHT_TWO){
        if (b.data.player_to_play ==WHITE &&  open_book_eighttwo_white.count(board_to_str(&(b.data)))){
            this->best_move = str_to_move(open_book_eighttwo_white[board_to_str(&(b.data))]);
            auto moves = b.get_legal_moves();
            if (moves.count(this->best_move)){return;}
        }
        if (b.data.player_to_play ==BLACK &&  open_book_eighttwo_black.count(board_to_str(&(b.data)))){
            this->best_move = str_to_move(open_book_eighttwo_black[board_to_str(&(b.data))]);
            auto moves = b.get_legal_moves();
            if (moves.count(this->best_move)){return;}
        } 
    }
    time_for_move = calc_time(this->time_left);
    st = std::chrono::high_resolution_clock::now();
    int limit = 10;
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
        return;
    }
    // int depth = 3;
    std::vector<U16> moves;
    std::copy(moveset.begin(), moveset.end(), std::back_inserter(moves));
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(moves.begin(), moves.end(), g);
    this->best_move = moves[0];
    if (this->time_left.count()<2000){
        // choose a random number if number is less then 0.5 then choose the best move
        // else choose a random move
        this->best_move = moves[0];
        return;
    }
    else if (this->time_left.count()<4000){
        if (b.data.player_to_play == WHITE)
        {
            int max_score = -1000000;
            U16 best_move = 0;
            for (auto m : moves) {
                Board b1 = Board(b);
                b1.do_move_(m);
                int score = evaluate_board(b1);
                if (score > max_score){
                    max_score = score;
                    best_move = m;
                }
            }
            this->best_move = best_move;
        }
        else{
            int min_score = 1000000;
            U16 best_move = 0;
            for (auto m : moves) {
                Board b1 = Board(b);
                b1.do_move_(m);
                int score = evaluate_board(b1);
                if (score < min_score){
                    min_score = score;
                    best_move = m;
                }
            }
            this->best_move = best_move;
        }
        return;
    }
    else if (this->time_left.count()<8000){
        if (b.data.player_to_play == WHITE)
        {
            int max_score = -1000000;
            U16 best_move = 0;
            for (auto m : moves) {
                Board b1 = Board(b);
                b1.do_move_(m);
                bool flag = is_capture(b,m);
                int score = minimax(b1, 1, -1000000, 1000000, false,limit,flag);
                if (score > max_score){
                    max_score = score;
                    best_move = m;
                }
            }
            this->best_move = best_move;
        }
        else{
            int min_score = 1000000;
            U16 best_move = 0;
            for (auto m : moves) {
                Board b1 = Board(b);
                b1.do_move_(m);
                bool flag = is_capture(b,m);
                int score = minimax(b1, 1, -1000000, 1000000, true,limit,flag);
                if (score < min_score){
                    min_score = score;
                    best_move = m;
                }
            }
            this->best_move = best_move;
        }
        return;
    }


    // Do iterative deepening minimax
    U16 best_move_till_now = moves[0];
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    if (b.data.player_to_play == WHITE)
    {
        // time of now in st in milliseconds
        auto et = st;
        priority_queue<pair<int,U16>> pq;
        for(int depth = 0;;depth++){
            int best_score = -999999;
            for (U16 move : moves) {
                // cout<<"Hi\n"<<search<<"\n";
                et = std::chrono::high_resolution_clock::now();
                bool search = (et -st ).count() < time_for_move;
                if (!search) {return;}
                Board* nb = new Board(b);
                nb->do_move_(move);
                bool flag = is_capture(b,move);
                int score = minimax(*nb, depth, alpha, beta, false,limit,flag );
                delete nb;
                pq.push({score,move});
                if (score > best_score) {
                    best_score = score;
                    best_move_till_now = move;
                }
            }
            et = std::chrono::high_resolution_clock::now();
            bool search = (et -st ).count() < time_for_move; 
            if (!search) return;
            moves.clear();
            while(!pq.empty()){
                moves.push_back(pq.top().second);
                pq.pop();
            }
            if (search) {
                this->best_move = best_move_till_now;
                // cout<<"Best Score at depth = "<<depth<<" is  = "<<best_score<<" by moving move = "<<move_to_str(best_move_till_now)<<"\n";
                if (best_score == 100000){
                    return;
                }

            }
            else return;
        }
    }

    else
    {   
        auto et = st;
        priority_queue<pair<int,U16>,std::vector<std::pair<int, U16> >, std::greater<std::pair<int, U16> > > pq;
        for(int depth = 0;;depth++){
            int best_score = 999999;
            for (U16 move : moves) {
                et = std::chrono::high_resolution_clock::now();
                bool search = (et -st ).count() < time_for_move;
                if (!search) return;
                Board* nb = new Board(b);
                nb->do_move_(move);
                bool flag = is_capture(b,move);
                int score = minimax(*nb, depth, alpha, beta, true,limit,flag);
                delete nb;
                pq.push({score,move});
                if (score < best_score) {
                    best_score = score;
                    best_move_till_now = move;
                }
            }
            et = std::chrono::high_resolution_clock::now();
            bool search = (et -st ).count() < time_for_move;
            if (!search) return;
            moves.clear();
            while(!pq.empty()){
                moves.push_back(pq.top().second);
                pq.pop();
            }
                
            if (search) {
                this->best_move = best_move_till_now;
                // cout<<"Best Score at depth = "<<depth<<" is  = "<<best_score<<" by moving move = "<<move_to_str(best_move_till_now)<<"\n";
                if (best_score == -100000){
                    return;
                }
            }
            else return;
        }
    }
    

}