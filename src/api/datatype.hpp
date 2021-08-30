#ifndef DATATYPE_DEF
#define DATATYPE_DEF

// #define	RAND_MAX	2147483647
#define	FILE_SIZE	1024 // GB
#define	VERT_SIZE	64 * 1024 * 1024 // 64M vertices in beg_pos buffer in preprocess
#define	EDGE_SIZE	256 * 1024 * 1024 // 256M edges in csr buffer in preprocess
#define	WALK_BUFFER_SIZE	4 * 1024 // most 1024 walks in a in-memory walk buffer
#define	MEM_BUDGET	44 * 1024 * 1024 // for 64GB memory machine

typedef uint32_t vid_t;
typedef uint64_t eid_t;
typedef uint64_t wid_t; //type of id of walks
typedef uint32_t bid_t; //type of id of blocks
typedef uint16_t hid_t; //type of id of hops
typedef uint8_t tid_t; //type of id of threads
typedef unsigned VertexDataType;
typedef unsigned long WalkDataType;

#endif