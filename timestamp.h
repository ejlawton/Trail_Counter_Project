
#ifndef TIMESTAMP
#define TIMESTAMP
//time stamp objects contain a count and a RTC value
typedef struct TIMESTAMP
{ 
    int count;
	uint32_t time;

}timestamp;

#endif