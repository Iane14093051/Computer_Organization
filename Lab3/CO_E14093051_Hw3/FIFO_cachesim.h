// See LICENSE for license details.

#ifndef _RISCV_CACHE_SIM_H
#define _RISCV_CACHE_SIM_H

#include "memtracer.h"
#include "common.h"
#include <cstring>
#include <string>
#include <map>
#include <unordered_map> 
#include <cstdint>
#include <iostream>
using namespace std;
class lfsr_t
{
 public:
  lfsr_t() : reg(1) {}
  lfsr_t(const lfsr_t& lfsr) : reg(lfsr.reg) {}
  uint32_t next() { return reg = (reg>>1)^(-(reg&1) & 0xd0000001); }
  void new_array(size_t ways, size_t sets)
  {
  	array = new uint64_t[sets*ways]();
  	for(int i=0;i<sets*ways;i++)
  	{
  		array[i]=0;
  	}
  }
  uint32_t FIFO(size_t ways) 
  {
	max_index++;
	max_index%=ways;
	/*int i=0;
	while(1)
	{
		if(i<ways&&array[idx*ways+i] != 0)
		{
			i++;
		}
		else if(i<ways&&array[idx*ways+i] == 0)
		{
			max_index=i;
			array[idx*ways+max_index] = 1;
			i++;
			break;
		}
		else if(i==ways)
		{
			for(int j=0;j<ways;j++)
			{
				array[idx*ways+j] = array[idx*ways+j+1];
			}
			array[idx*ways+ways-1] = 0;
			max_index=ways-1;
			break;
		}
	}*/ 
	return max_index;
  } 
  uint64_t FIFO_full(size_t ways,uint64_t locate)
  {

  	  	map<uint64_t, int>::iterator start=full_ma.begin();
  	  	int min=start->second;
  	  	uint64_t index=start->first;
  	  	int temp =0;
  	  	for(map<uint64_t, int>::iterator i=full_ma.begin();i!=full_ma.end();i++)
  	  	{
  	  		if(min>i->second)
  	  		{
  	  			index = i->first;
  	  			min = i->second;
  	  			temp =distance(start,i);
			}
		}
		
		

  		full_ma.erase(index);

	   return temp;

  } 
  void insert(uint64_t line)
  {
	max_ren++;
	full_ma[line]=max_ren;
  }
// private:
  uint32_t reg;
  int max_index = -1;
  map<uint64_t, int> full_ma;
  uint64_t* array;
  int max_ren = 0;
  int full = 0;
  uint64_t iter=1;
  
};

class cache_sim_t
{
 public:
  cache_sim_t(size_t sets, size_t ways, size_t linesz, const char* name);
  cache_sim_t(const cache_sim_t& rhs);
  virtual ~cache_sim_t();

  void access(uint64_t addr, size_t bytes, bool store);
  void clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval);
  void print_stats();
  void set_miss_handler(cache_sim_t* mh) { miss_handler = mh; }
  void set_log(bool _log) { log = _log; }

  static cache_sim_t* construct(const char* config, const char* name);

 protected:
  static const uint64_t VALID = 1ULL << 63;
  static const uint64_t DIRTY = 1ULL << 62;

  virtual uint64_t* check_tag(uint64_t addr);
  virtual uint64_t victimize(uint64_t addr);

  lfsr_t lfsr;
  cache_sim_t* miss_handler;

  size_t sets;
  size_t ways;
  size_t linesz;
  size_t idx_shift;

  uint64_t* tags;
  
  uint64_t read_accesses;
  uint64_t read_misses;
  uint64_t bytes_read;
  uint64_t write_accesses;
  uint64_t write_misses;
  uint64_t bytes_written;
  uint64_t writebacks;

  std::string name;
  bool log;

  void init();
};

class fa_cache_sim_t : public cache_sim_t
{
 public:
  fa_cache_sim_t(size_t ways, size_t linesz, const char* name);
  uint64_t* check_tag(uint64_t addr);
  uint64_t victimize(uint64_t addr);
 private:
  static bool cmp(uint64_t a, uint64_t b);
  std::map<uint64_t, uint64_t> tags;
};

class cache_memtracer_t : public memtracer_t
{
 public:
  cache_memtracer_t(const char* config, const char* name)
  {
    cache = cache_sim_t::construct(config, name);
  }
  ~cache_memtracer_t()
  {
    delete cache;
  }
  void set_miss_handler(cache_sim_t* mh)
  {
    cache->set_miss_handler(mh);
  }
  void clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval)
  {
    cache->clean_invalidate(addr, bytes, clean, inval);
  }
  void set_log(bool log)
  {
    cache->set_log(log);
  }

 protected:
  cache_sim_t* cache;
};

class icache_sim_t : public cache_memtracer_t
{
 public:
  icache_sim_t(const char* config) : cache_memtracer_t(config, "I$") {}
  bool interested_in_range(uint64_t UNUSED begin, uint64_t UNUSED end, access_type type)
  {
    return type == FETCH;
  }
  void trace(uint64_t addr, size_t bytes, access_type type)
  {
    if (type == FETCH) cache->access(addr, bytes, false);
  }
};

class dcache_sim_t : public cache_memtracer_t
{
 public:
  dcache_sim_t(const char* config) : cache_memtracer_t(config, "D$") {}
  bool interested_in_range(uint64_t UNUSED begin, uint64_t UNUSED end, access_type type)
  {
    return type == LOAD || type == STORE;
  }
  void trace(uint64_t addr, size_t bytes, access_type type)
  {
    if (type == LOAD || type == STORE) cache->access(addr, bytes, type == STORE);
  }
};

#endif
