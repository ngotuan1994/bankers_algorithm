//
//  customer.h
//  bankers
//
//

#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

#include "bank.h"
#include "ext_vector.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iomanip>

class Bank;

extern pthread_mutex_t mutex_;



class Customer {
public:
//  Customer() = default;
  Customer(int index, const ext_vector<int>& allocate, const ext_vector<int>& maximum,
                  Bank* bank_=nullptr)
  : idx(index), alloc(allocate), max_(maximum), need(max_ - alloc), bank(bank_) {
      srand (time(NULL));
  }
  
  int get_id() const { return idx; }
  pthread_t* get_threadid() { return &thread_id; }
  
  Bank* get_bank() const { return bank; }
  ext_vector<int> get_max() const { return max_; }
  
  const ext_vector<int>& allocated() const { return alloc; }
  
  bool needs_met() const { return alloc.operator>= (max_); }
  bool needs_exceeded(const ext_vector<int>& req) const { return alloc.operator+(req) > max_; }
  
  void alloc_req(  const ext_vector<int>& req) { alloc.operator+= (req);  need.operator-= (req); }
  void dealloc_req(const ext_vector<int>& req) { alloc.operator-= (req);  need.operator+= (req); }
  
  void release_all_resources() {
    pthread_mutex_lock(&mutex_);
    std::cout << ">>>>>>>>>>>>>>> Customer p#" << idx << " has all its resources! RELEASING [" << max_ <<"],allocated=[";
    ext_vector<int> zero(alloc.size(), 0);
    alloc = max_ = need = zero;
    std::cout << alloc << "] " << "<<<<<<<<<<<<<<<<<\n";
    pthread_mutex_unlock(&mutex_);
  }

  void show() const {
    std::string tmp;
    for(int i =0 ; i < alloc.size();++i)
      tmp += "----";
    if(!alloc.is_zero())
      std::cout << "P#" << std::setw(3) << idx << "    [" << alloc << " ]    [" << max_ << " ]    [" << need << " ]\n";
    else
      std::cout << "P#" << std::setw(3) << idx << "   " << tmp << "   " << tmp << "  " << tmp << "\n";
  }

  friend std::ostream& operator<<(std::ostream& os, const Customer& c) {
    c.show();
    return os;
  }
  
  ext_vector<int> create_req() {
    ext_vector<int> req;
    for (size_t i = 0; i < alloc.size(); ++i) {
      if (need[i] == 0) { req.push_back(0);  continue; }
      int val = (rand() % need[i]) + 1;
      req.push_back(val);
    }
    return req;
  }
  
private:
  int idx;
  ext_vector<int> alloc;
  ext_vector<int> max_;
  ext_vector<int> need;
  pthread_t        thread_id;
  
  Bank* bank;
};

#endif /* Customer_h */
