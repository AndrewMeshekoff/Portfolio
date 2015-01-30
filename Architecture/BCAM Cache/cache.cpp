//amesh001, Andrew Meshekoff

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

#define BLOCK_SIZE 16

using namespace std;


int main () {
	int cache_size = 0;
	int asctv = 0;
	long long num_sets = 0;
	int misses = 0;
	
	vector <long long> cache;
	for(int i=0; i<16384; i++)
		cache.push_back(0);
	
	ifstream input("trace");
	
	vector<long long> trace;
	long long mem;
	
	while(input >> std::hex >>mem)
		trace.push_back(mem);
	int trace_size = trace.size();
		
	
		
	//LRU
	long long index = 0;
	long long tag = 0;
	int tag_shift = 0;
	
	cout << "\t  LRU Replacement Policy" << endl;
	cout << "\t1024" << "\t2048" << "\t4096" << "\t8192" << "\t16384" << endl;
	
	for (int i = 1; i<=8; i*=2) {
		asctv = i;
		cout << asctv;
		for (int j = 1024; j<=16384; j*=2) {
			cache_size = j;
			num_sets = (cache_size/BLOCK_SIZE)/asctv;
			misses = 0;
			
			for(int k=0; k<j; k++)// clear cache
				cache[k] = 0;
			
			tag_shift = 0;
			for(int k=num_sets-1; k>0; k = k >> 1)// calculate index shift
				tag_shift++;
			
			for(int k=0; k<trace_size; k++) {// input memory
				index = (trace[k] >> 4)&(num_sets-1);
				tag = trace[k] >> (tag_shift + 4);
				
				bool tag_found = false;
				for(int set_index = 0; (set_index < asctv); set_index++) {// hit
					if (cache[index*asctv + set_index] == tag) {
						for(int shifter = set_index; shifter < asctv-1; shifter++) {
							long long temp = cache[index*asctv + shifter];
							cache[index*asctv + shifter] = cache[index*asctv + shifter + 1];
							cache[index*asctv + shifter + 1] = temp;
						}
						tag_found = true;
					}
				}
				if(!tag_found) { // miss
					misses++;
					for(int shifter = 0; shifter < asctv-1; shifter++)
						cache[index*asctv + shifter] = cache[index*asctv + shifter + 1];
					cache[index*asctv + asctv-1] = tag;
				}
			}
			
			double miss_perc = ((double) misses/(double) trace_size) * 100.0;
			cout << "\t" << setfill('0') << setw(4) << setprecision(2) << fixed << miss_perc;
		}
		
		cout << endl;
	}
	
	cout << endl;
	//FIFO
	
	cout << "\t  FIFO Replacement Policy" << endl;
	cout << "\t1024" << "\t2048" << "\t4096" << "\t8192" << "\t16384" << endl;
	
	for (int i = 1; i<=8; i*=2) {
		asctv = i;
		cout << asctv;
		for (int j = 1024; j<=16384; j*=2) {
			cache_size = j;
			num_sets = (cache_size/BLOCK_SIZE)/asctv;
			misses = 0;
			
			for(int k=0; k<j; k++)// clear cache
				cache[k] = 0;
			
			tag_shift = 0;
			for(int k=num_sets-1; k>0; k = k >> 1)// calculate index shift
				tag_shift++;
			
			for(int k=0; k<trace_size; k++) {// input memory
				index = (trace[k] >> 4)&(num_sets-1);
				tag = trace[k] >> (tag_shift + 4);
				
				bool tag_found = false;
				for(int set_index = 0; (set_index < asctv); set_index++) // hit
					if (cache[index*asctv + set_index] == tag)
						tag_found = true;
				if(!tag_found) { // miss
					misses++;
					for(int shifter = 0; shifter < asctv-1; shifter++)
						cache[index*asctv + shifter] = cache[index*asctv + shifter + 1];
					cache[index*asctv + asctv-1] = tag;
				}
			}
			
			double miss_perc = ((double) misses/(double) trace_size) * 100.0;
			cout << "\t" << setfill('0') << setw(4) << setprecision(2) << fixed << miss_perc;

		}
		
		cout << endl;
	}
	
	
	
	return 0;
}
