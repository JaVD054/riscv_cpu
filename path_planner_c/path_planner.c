#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define V 30
#define INF 0xf

#ifdef __linux__ // for host pc

    #include <stdio.h>

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


int bit_position(uint32_t *n, uint8_t *i) {
    // printf("n: %x\n", n );
    return ((*n >> (29-*i)) & 1);
}
void bit_load(uint32_t *n, uint8_t *i){
    *n |= 0x20000000 >> *i;
}


uint8_t array_index(uint32_t *arr, uint8_t n) {
    return (arr[(n & 0xf8)>>3]&(0XF0000000>>((n&0x7)*4)))>>(~(n&0x7)*4); 
} 

uint8_t array_index8(uint32_t *arr, uint8_t n) {
    return (arr[(n & 0xfc)>>2]&(0XFF000000>>((n&0x3)*8)))>>(~(n&0x3)*8); 
} 

void array_write(uint32_t *arr, uint8_t *n, uint8_t val) {
    arr[(*n & 0xf8)>>3] = (arr[(*n & 0xf8)>>3]&~(0XF0000000>>((*n&0x7)*4))) | (val << (~(*n&0x7)*4));
}

void array_write8(uint32_t *arr, uint8_t *n, uint8_t val) {
    arr[(*n & 0xfc)>>2] = (arr[(*n & 0xfc)>>2]&~(0XFF000000>>((*n&0x3)*8))) | (val << (~(*n&0x3)*8));
}

uint8_t minDistance(uint32_t dist[], uint32_t *sptSet)
{
	int min = INF, min_index;

	for (uint8_t v = 0; v < V; v++)
		if (bit_position(sptSet,&v) == false && array_index (dist,v) <= min)
			min = array_index (dist,v), min_index = v;

	return min_index;
}


// main function
int main(int argc, char const *argv[]) {

    #ifdef __linux__

        const uint8_t START_POINT   = atoi(argv[1]);
        const uint8_t END_POINT     = atoi(argv[2]);
        uint8_t NODE_POINT          = 0;
        uint8_t CPU_DONE            = 0;

    #else
        // Address value of variables are updated for RISC-V Implementation
        #define START_POINT         (* (volatile uint8_t * ) 0x02000000)
        #define END_POINT           (* (volatile uint8_t * ) 0x02000004)
        #define NODE_POINT          (* (volatile uint8_t * ) 0x02000008)
        #define CPU_DONE            (* (volatile uint8_t * ) 0x0200000c)

    #endif

    // array to store the planned path

    // index to keep track of the path_planned array

    // NODE_POINT = 0;
    // NODE_POINT = 1;
    // NODE_POINT = 2;
    // NODE_POINT = 8;
    // NODE_POINT = 7;
    // CPU_DONE = 1;

    // ############# Add your code here #############

    uint32_t visited = 0x0;
    uint32_t prev[8] ={0};
    // prev = 0x0200000c;
    uint32_t dist[4] = {0}; 
    // uint32_t* map = (uint32_t*) malloc(sizeof(uint32_t)*30);
    uint32_t map[30];

    map[0] = 0b010000000000000000000000000000;
    map[1] = 0b101000000000000000000000000001;
    map[2] = 0b010100001000000000000000000000;
    map[3] = 0b001010000000000000000000000010;
    map[4] = 0b000101100000000000000000000000;
    map[5] = 0b000010000000000000000000000000;
    map[6] = 0b000010010000000000000000000000;
    map[7] = 0b000000101000000000000000000000;
    map[8] = 0b001000010100100000000000000000;
    map[9] = 0b000000001011000000000000000000;
    map[10] = 0b000000000100000000000000000000;
    map[11] = 0b000000000100000000000000000000;
    map[12] = 0b000000001000010000010000000000;
    map[13] = 0b000000000000101000000000000000;
    map[14] = 0b000000000000000110000000000000;
    map[15] = 0b000000000000001000000000000000;
    map[16] = 0b000000000000001001100000000000;
    map[17] = 0b000000000000000010000000000000;
    map[18] = 0b000000000000000010010000000000;
    map[19] = 0b000000000000100000101000000000;
    map[20] = 0b000000000000000000010100100001;
    map[21] = 0b000000000000000000001011000000;
    map[22] = 0b000000000000000000000100000000;
    map[23] = 0b000000000000000000000100000000;
    map[24] = 0b000000000000000000001000010000;
    map[25] = 0b000000000000000000000000101000;
    map[26] = 0b000000000000000000000000010110;
    map[27] = 0b000000000000000000000000001000;
    map[28] = 0b000100000000000000000000001001;
    map[29] = 0b010000000000000000001000000010;


    for (uint8_t i = 0; i < 4; ++i) {
        dist[i] = 0xffffffff;
    }

    array_write(dist, &START_POINT, 0);
    array_write8(prev, &START_POINT, 0xff);



    for (uint8_t i = 0; i < V - 1; i++) {
		uint8_t u = minDistance(dist, &visited);

        //break if the destination is reached
        if (u == END_POINT)
            break;
        //mark the vertex as visited
		bit_load(&visited,&u);

        // updating the distance of the adjacent unvisited vertices
		for (uint8_t v = 0; v < V; v++)
			if (!bit_position (&visited,&v) && bit_position (&map[u],&v)   // if the vertex is not visited (value of current v has never been the value of u) 
                                            //and there is an edge between u and v
				&& array_index(dist,u) != INF  // if the distance of u is not infinity
				&& array_index(dist,u) + bit_position (&map[u],&v) < array_index(dist,v)) // sum of distance to u and edge weight of v-u is 
                                                    //less than the distance to v (v is not visited)
                {
                    array_write(dist,&v,array_index(dist,u) + bit_position (&map[u],&v)); //update the distance of v     
                    // printf("dist: %d\n", array_index(dist,v));
                    array_write8(prev,&v, u); //update the parent of v
                }
	}
    // free(dist);
    // free(&visited);
    // free(map);

    uint8_t currentVertex = END_POINT;
    uint8_t idx = 0;

    map[(idx)++] = currentVertex;
    while (currentVertex != START_POINT) {
        map[(idx)++]= currentVertex = array_index8(prev,currentVertex);
    }
    // ##############################################

    // the node values are written into data memory sequentially.
    for (int i = --idx; i >=0; i--) {
        NODE_POINT = map[i];
    }

    // Path Planning Computation Done Flag
    CPU_DONE = 1;

    #ifdef __linux__    // for host pc

        _put_str("######### Planned Path #########\n");
        for (int i = idx; i >=0; i--) {
            _put_value(map[i]);
        }
        _put_str("################################\n");

    #endif

    return 0;
}
