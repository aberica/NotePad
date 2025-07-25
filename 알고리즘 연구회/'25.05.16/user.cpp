struct ROAD {
	int city1, city2, status, roadid;	
};

struct FREIGHT {
	int pos, dest, weight ;
};

extern void getroad(ROAD road[]) ;
extern void getfreight(FREIGHT freight[]);

extern bool move(int RoadID, int repair) ;
extern bool load(int FreightID) ;
extern bool unload();

void process()
{
	
}
