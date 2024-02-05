/*
# Team ID:          1176
# Theme:            Astro Bot
# Author List:      Sriram, Javeed, Abhishek
# Filename:         t1b_path_planner.c
# File Description: path finder for the robot
# Global Variables:  START_POINT, END_POINT, NODE_POINT, CPU_DONE
*/



#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#define V 30
#define INF 0xf

#ifdef __linux__ // for host pc


    void _put_byte(char c) { putchar(c); }

    void _put_str(char *str) {
        while (*str) {
            _put_byte(*str++);
        }
    }

    void print_output(uint8_t num) {
        if (num == 0) {
            putchar('0'); // if the number is 0, directly print '0'
            _put_byte('\n');
            return;
        }

        if (num < 0) {
            putchar('-'); // print the negative sign for negative numbers
            num = -num;   // make the number positive for easier processing
        }

        // convert the integer to a string
        char buffer[20]; // assuming a 32-bit integer, the maximum number of digits is 10 (plus sign and null terminator)
        uint8_t index = 0;

        while (num > 0) {
            buffer[index++] = '0' + num % 10; // convert the last digit to its character representation
            num /= 10;                        // move to the next digit
        }

        // print the characters in reverse order (from right to left)
        while (index > 0) { putchar(buffer[--index]); }
        _put_byte('\n');
    }

    void _put_value(uint8_t val) { print_output(val); }

#else  // for the test device

    void _put_value(uint8_t val) { }
    void _put_str(char *str) { }

#endif

int is_node_connected(uint32_t *arr, uint8_t *n) {
    for (uint8_t i = 0; i < 4; i++) {
        if (((*arr>>(i*8))&0xff) == *n) return 1;
    } 
    return 0;
}

//to find the bit value at a particular position
int bit_position(uint32_t *n, uint8_t *i) {
    return ((*n >> (*i)) & 1);
}
//to set the bit value at a particular position
void bit_load(uint32_t *n, uint8_t *i){
    *n |= 1 << *i;
}

//to read the 4bit value at a particular position
uint8_t array_index(uint32_t *arr, uint8_t n) {
    return (arr[(n & 0xf8)>>3]&(0XF0000000>>((n&0x7)*4)))>>(~(n&0x7)*4); 
} 
//to read the byte value at a particular position 
uint8_t array_index8(uint32_t *arr, uint8_t n) {
    return (arr[(n & 0xfc)>>2]&(0XFF000000>>((n&0x3)*8)))>>(~(n&0x3)*8); 
} 

//to write the 4bit value at a particular position
void array_write(uint32_t *arr, uint8_t *n, uint8_t val) {
    arr[(*n & 0xf8)>>3] = (arr[(*n & 0xf8)>>3]&~(0XF0000000>>((*n&0x7)*4))) | (val << (~(*n&0x7)*4));
}
//to write the byte value at a particular position
void array_write8(uint32_t *arr, uint8_t *n, uint8_t val) {
    arr[(*n & 0xfc)>>2] = (arr[(*n & 0xfc)>>2]&~(0XFF000000>>((*n&0x3)*8))) | (val << (~(*n&0x3)*8));
}

// A utility function to find the vertex with minimum distance value from src and not in sptSet
uint8_t minDistance(uint32_t dist[], uint32_t *sptSet)
{
	int min = INF, min_index;

	for (uint8_t v = 0; v < V; v++)
		if (bit_position(sptSet,&v) == false && array_index (dist,v) <= min)
			min = array_index (dist,v), min_index = v;

	return min_index;
}

uint8_t turn_dir(uint32_t currentnode,uint8_t* prev_node,uint8_t* next_node){
    if(*prev_node == 0xff) return 0x0;
    for (uint8_t i = 0; i < 4; i++){
        if (((currentnode>>(i*8))&0xff) == *next_node) {
            for (uint8_t j = 0; j < 4; j++){
                if (((currentnode>>(((i+j)&0b11)*8))&0xff) == *prev_node) {
                    return (j+2)&0b11;
                }
            }
            return 0xf0;         
        } 
    }
    return 0xff;   
}


// main function
int main(int argc, char const *argv[]) {

    #ifdef __linux__

        uint8_t START_POINT   = 0;//atoi(argv[1]);
        uint8_t END_POINT     = 20;//atoi(argv[2]);
        uint32_t NODE_POINT          = 0;
        uint8_t CPU_DONE            = 0;
        uint8_t PREV_NODE            = 255;

    #else
        // Address value of variables are updated for RISC-V Implementation
        #define START_POINT         (* (volatile uint8_t * ) 0x02000000)
        #define END_POINT           (* (volatile uint8_t * ) 0x02000004)
        #define NODE_POINT          (* (volatile uint8_t * ) 0x02000008)
        #define CPU_DONE            (* (volatile uint8_t * ) 0x0200000c)
        #define PREV_NODE            (* (volatile uint8_t * ) 0x02000010)

    #endif


    // index to keep track of the path_planned array
    uint8_t idx = 0;


    
    // check if the start and end points are valid
    if(!(30>START_POINT && START_POINT>=0 && 30>END_POINT && END_POINT>=0)) return 0;

    uint32_t visited = 0x0; // visited vertices represented as a bit vector

    #ifdef __linux__    // for host pc
        uint32_t prev[8]; // array to store the parent of each vertex, each 32 bit integer stores 8 vertices
        uint32_t dist[4]; // array to store the distance of each vertex, each 32 bit integer stores 4 vertices
        uint32_t path_planned[30]; // array to store the planned path and adjacency matrix of the graph
    #else
        uint32_t *path_planned =0x02000034;      
        uint32_t *prev = 0x02000014; 
        uint32_t *dist = 0x020000AC;
    #endif

    // adjacency matrix of the graph
    path_planned [0] = 0x01ffffff;
    path_planned [1] = 0x001dff02;
    path_planned [2] = 0x0108ff03;
    path_planned [3] = 0x02ff041c;
    path_planned [4] = 0x030506ff;
    path_planned [5] = 0x04ffffff;
    path_planned [6] = 0x04ff07ff;
    path_planned [7] = 0x08ff06ff;
    path_planned [8] = 0x0702090c;
    path_planned [9] = 0x080aff0b;
    path_planned [10] = 0x09ffffff;
    path_planned [11] = 0x09ffffff;
    path_planned [12] = 0x0813ff0d;
    path_planned [13] = 0x0eff0cff;
    path_planned [14] = 0x0d0f10ff;
    path_planned [15] = 0x0effffff;
    path_planned [16] = 0x0e1112ff;
    path_planned [17] = 0x10ffffff;
    path_planned [18] = 0x13ff10ff;
    path_planned [19] = 0x12ff0c14;
    path_planned [20] = 0x13151d18;
    path_planned [21] = 0x1417ff16;
    path_planned [22] = 0x15ffffff;
    path_planned [23] = 0x15ffffff;
    path_planned [24] = 0x19ff14ff;
    path_planned [25] = 0x1aff18ff;
    path_planned [26] = 0x191b1cff;
    path_planned [27] = 0x1affffff;
    path_planned [28] = 0x1aff1d03;
    path_planned [29] = 0x1cff1401;

    // initialize the distance of all vertices to infinity
    for (uint8_t i = 0; i < 4; i++) {
        dist[i] = 0xffffffff;
    }

    // initialize the distance of the start vertex to 0
    array_write(dist, &START_POINT, 0);
    // initialize the parent of the start vertex to 0xff
    array_write8(prev, &START_POINT, 0xff);



    for (uint8_t i = 0; i < V - 1; i++) {
        // pick the vertex with the minimum distance from the set of unvisited vertices
		uint8_t u = minDistance(dist, &visited);

        //break if the destination is reached
        if (u == END_POINT) break;

        //mark the vertex as visited
		bit_load(&visited,&u);

        // updating the distance of the adjacent unvisited vertices
		for (uint8_t v = 0; v < V; v++){
            bool weight = is_node_connected(&path_planned[u],&v);
            uint8_t dist_u = array_index(dist,u);

			if (!bit_position (&visited,&v) && weight   
                                // if the vertex is not visited (value of current v has never been the value of u) 
                                //and there is an edge between u and v
				&& dist_u != INF  
                                // if the distance of u is not infinity
				&& dist_u + weight < array_index(dist,v)) 
                                // sum of distance to u and edge weight of v-u is 
                                //less than the distance to v (v is not visited)
                {
                    array_write(dist,&v,dist_u + weight); //update the distance of v     
                    array_write8(prev,&v, u); //update the parent of v
                }}
	}

    // backtracking the path from the destination to the start
    uint8_t current_vertex = END_POINT;
    array_write8(dist,&idx,END_POINT);
    // path_planned[(idx)++] = current_vertex;
    while (current_vertex != START_POINT) {
        idx++;
        current_vertex = array_index8(prev,current_vertex);
        array_write8(dist,&idx,current_vertex);
        // path_planned[(idx)]= current_vertex = array_index8(prev,current_vertex);
    }


    uint32_t prev_node;
    CPU_DONE = 2;
    // the node values and turn direction are written into data memory sequentially.
    for (int i = idx - 1; i >=0; i--) {
        // wait for the CPU to read the previous node value
        if (CPU_DONE == 2) {
            prev_node = PREV_NODE;
            CPU_DONE = 0;
        }
        NODE_POINT = array_index8(dist,i);
        //write the turn direction into START_POINT data memory
        START_POINT = turn_dir(path_planned[current_vertex],&prev_node,&NODE_POINT);
        //
        prev_node = current_vertex;
        current_vertex = NODE_POINT;
        
    }

    // Path Planning Computation Done Flag
    CPU_DONE = 1;

    #ifdef __linux__    // for host pc

        _put_str("######### Planned Path #########\n");
        for (int i = idx; i >=0; i--) {
            _put_value(array_index8(dist,i));
        }
        _put_str("################################\n");

    #endif

    return 0;
}
