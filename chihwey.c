/*
    Course: CS 245 Spring 2016
    Name:  chih-wei yang
    Student id: 34564196
    email address: chihwey@uci.edu
    Assignment: test2
    Filename : chihwey_34564916.c,input.txt(test file)

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>


typedef struct place{
    char place[1024];
    char place_index[1024];
    char token;
    int sub_record;
} place;

typedef struct transations{
    char transation[1024];
    char transation_index[1024];
    int need_token;
} transations;

typedef struct edges{
    char edge[1024];
    char in[1024];
    char out[1024];
}edges;

int max_cycle,total_edges,total_place,total_trans;
int cycle_count;
void create_place_table(int total_place);
edges *edges_proc(char *line);
place *place_proc(char *line);
transations *trans_proc(char *line);
void read_file(char filename[]);
char * transations_check(char *start_trans);
void transition_record();
int ptoken(int tans_index);
int p_token_helper(char * place);
char * check_go(int max_cycle);
void sub_token_helper(char * sub_token_place);
void sub_token(int in_trans);
void add_token(int in_trans);
void add_token_helper(char * sub_token_place);
void sub_record();
void print_result(int * place_token_table, int act_count);
int in_talbe_helper(char * find_place);



void magic_box(int total_trans,int total_place,int total_edges,
  int max_cycle);



int place_list[1024];
static place * place_data[1024];
static edges * edges_data[1024];
static transations * trans_data[1024];

void read_file(char filename[]){
    FILE * fp;
    char title;
    //char fgets[1024];
    char in_data[1024];
    int num_state = 0;
    int num_edges = 0;
    int num_trans = 0;
    fp = fopen(filename,"r");
    if(fp == NULL){
        printf("open file error\n");
        exit(1);
    }
    else{
        while(fgets(in_data,1024,fp) != NULL){
            //fgets(fgets,3,fp);
            //printf("%s\n",in_data);

            title = in_data[0];
            if(title=='p'){
                //printf("number_state:%d\n",num_state);

                place_data[num_state] = place_proc(in_data);

                num_state ++;

            }
            else if(title == 'e'){
                edges_data[num_edges] = edges_proc(in_data);
                num_edges ++;
            }
            else{
                trans_data[num_trans] = trans_proc(in_data);
                num_trans ++;
            }
        }

    }
    total_place = num_state;
    total_edges = num_edges;
    total_trans = num_trans;
    fclose(fp);

}
transations *trans_proc(char *line){
    char * result3;
    transations * temp;
    int col_index = 0;
    temp = malloc(sizeof(struct transations));
    if(line != NULL){
        //string = line;
        while((result3 = strsep(&line," ")) != NULL){
            if(col_index == 0){

                strncpy( temp->transation, result3,strlen(result3));

            }
            else{
                strncpy( temp->transation_index, result3,strlen(result3));
            }
            col_index++;
        }
        free(result3);
    }
    return temp;
}
edges *edges_proc(char *line){
    char * result;
    edges * temp;
    int col_index = 0;
    temp = malloc(sizeof(struct edges));
    if(line != NULL){
        //string = line;
        while((result = strsep(&line," ")) != NULL){
            if(col_index == 0){

                strncpy( temp->edge, result,strlen(result));

            }
            else if(col_index ==1){

                strncpy( temp->in, result,strlen(result));
            }
            else{
                strncpy( temp->out, result,strlen(result));
            }
            col_index++;
        }
        free(result);
    }
    return temp;
}

place *place_proc(char *line){
    //char *split = " ";
    char * result2;
    place * temp;
    int col_index = 0;
    temp = malloc(sizeof(struct place));

    if(line != NULL){
        //string = line;
        while((result2 = strsep(&line," ")) != NULL){
            if(col_index == 0){

               // temp->place = result2;
               strncpy( temp->place,result2, strlen(result2));
            }
            else if (col_index ==1){

                //temp->place_index = result2;
                strncpy( temp->place_index,result2, strlen(result2));

            }
            else{
                //temp->token = atoi(result2);
                //strncpy( temp->token, result2, strlen(result2));
                temp->token = atoi(result2);
            }
            col_index++;
        }
        free(result2);
    }
    return temp;
}

void transition_record(){
    int i , j;
    int compare_index;
    int num_need_token;
    char *start_place;
    char *end_place;
    num_need_token = 0;
    for(i = 0; i < total_trans;i++){
        for(j = 0; j < total_edges ; j++){
            end_place = edges_data[j]->out;
            compare_index = strncmp(end_place,trans_data[i]->transation_index,2);
            if(compare_index == 0){
                num_need_token++;
            }
        }
        trans_data[i]->need_token = num_need_token;
        //printf("trans:%s   token_num:%d\n",trans_data[i]->transation_index,trans_data[i]->need_token );
        num_need_token = 0;
    }
}



void create_place_table(int total_place){
  //int temp_place_list[total_place];
  //place_list = （int *)malloc(total_place * sizeof(int));
  for(int i = 0; i < total_place; i++){
    place_list[i] = place_data[i]->token;
  }
}

int in_talbe_helper(char * find_place){
  int compare_index,place_flag;
  //printf("%s\n",find_place);
  for(int j = 0; j < total_place ; j++){
    //printf("place_index:%s\n",place_data[j]->place_index);
    compare_index = strncmp(place_data[j]->place_index,
                        find_place,1);
    if(compare_index == 0){
      place_flag = j;
    }
    else{
      //continue;
    }
  }
  return place_flag;
}


void magic_box(int total_tans,int total_place , int total_edges
,int max_cycle){
    int t,s,compare_index,y,k,i;
    int emmit,index_for;
    int act_count;
    /*create in table*/
    int in_table[total_tans][total_place];
    int out_table[total_tans][total_place];
    int place_token_table[total_place];

    /*create place_token_table*/
    for(i = 0; i < total_place; i++){
      place_token_table[i] = place_data[i]->token;
    }
      /*initial talbe*/
    for(t = 0; t < total_tans ; t++){
      for(s = 0; s < total_place; s++){
        in_table[t][s] = 0;
        out_table[t][s] = 0;
      }
    }
      /*insert element*/
    for(k = 0; k < total_edges ; k++){
      for(i = 0 ; i < total_trans ; i++){
        compare_index = strncmp(edges_data[k]->out,
                            trans_data[i]->transation_index,2);
        //printf("edges:%s\t trans:%s\n",edges_data[k]->out,trans_data[i]->transation_index);
        if(compare_index == 0){
          //printf("trans_name: %s\t loc:%d\n",edges_data[k]->out,i);
          y = in_talbe_helper(edges_data[k]->in);
          in_table[i][y] += 1;
          //exit(0);
        }
      }
    }
    /*create out table*/

    for(k = 0; k < total_edges ; k++){
      for(i = 0 ; i < total_trans ; i++){
        compare_index = strncmp(edges_data[k]->in,
                            trans_data[i]->transation_index,2);
        //printf("edges:%s\t trans:%s\n",edges_data[k]->out,trans_data[i]->transation_index);
        if(compare_index == 0){
          //don't coufuse it, since the function is same as in_table
          y = in_talbe_helper(edges_data[k]->out);
          out_table[i][y] += 1;
        }
      }
    }

    /*compare in table and place table*/

    //start work//
    int jump_flag,fail_number;
    jump_flag = 0;
    fail_number = 0;
    //fail_number < total_trans
    while(fail_number < total_trans){
      if(jump_flag == 1 || fail_number == total_trans){
          break;
      }
      for( i = 0 ;i <total_tans; i++){
        if(act_count >= max_cycle){
          //printf("act_count:%d\n",act_count);
          jump_flag = 1;
          break;
        }
        emmit = 0;
        for(int j = 0; j < total_place; j++){
          if(in_table[i][j] <= place_token_table[j]){
              emmit++;
          }
        }
        if(emmit == total_place){
          act_count++;
          fail_number = 0;
          for(k = 0; k < total_place; k++){
            //Subtraction place_token_table by in_table
            place_token_table[k] = place_token_table[k]-in_table[i][k];
            //add place_token_table by in_table
            place_token_table[k] = place_token_table[k]+out_table[i][k];
            /*show result*/
          }
          print_result(place_token_table,act_count);
        }
        else{
          fail_number++;
        }
      }
    }   
   
}
void print_result(int *place_token_table,int act_count){
    printf("------------result----------phase:%d\n",act_count);
     for(int s = 0; s < total_place; s++){

        printf("\n place: %s\t",place_data[s]->place_index);
        printf("token: %d\n",place_token_table[s]);
    }

}
int main(int argc, char * argv[]){
    int max_cycle,compare_index;
    cycle_count = 0;
    if(argc <= 2){
      printf("too few arguments........pleaes try again\n");
      exit(1);
    }
    max_cycle = atoi(argv[2]);
    //printf("max_cycle:%d\n",max_cycle);
    read_file(argv[1]);

    magic_box(total_trans,total_place,total_edges,max_cycle);
    return 0;
}
