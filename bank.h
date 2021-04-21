//
//  bank.h
//
//

#ifndef __BANK_H__
#define __BANK_H__

#include "customer.h"


class Bank {
public:
  Bank() = default;
  Bank(const ext_vector<int>& available) : avail(available), customers() { }
  
  ext_vector<int> get_avail() const { return avail; }
  bool is_avail(const ext_vector<int>& req) const { return req.operator<=(avail); }
  int vector_check(ext_vector<int> a,ext_vector<int> b, int len){
    int i; 
    for (i = 0; i < len; i++)
        if (a[i] > b[i])
          break; 
    if (i == len)
        return 1; 
    else 
        return 0; 
}
  bool is_safe(int id, const ext_vector<int>& req) {
      const Customer* c = customers[id];
      bool flag = 1;
      for( int i = 0 ; i < req.size(); ++i){
        if(avail[i] < req[i]){
          flag = false;
        }
        if(c->needs_exceeded(req)){
          flag = false;
        }
        ext_vector<int> tmp_avail = avail.operator-(req);
        ext_vector<int> tmp_need = c->get_max() - (req + c->allocated());
        if(flag){
          for(int j =0 ; j < tmp_need.size() ; ++ j){
            if (tmp_avail[j] < tmp_need[j]){
              return false;
            }
          }
        }
      }
      ext_vector<Customer*> c1 = customers;

      bool check = true;
      if(flag){
        ext_vector<int> work = avail - req;
        c1[id]->alloc_req(req);
        ext_vector<int> finish(c1.size(),0);
      
        for(int i = 0 ; i < c1.size() ; ++i){
          bool possible = true;
          ext_vector<int> need = c1[i]->get_max() - c1[i]->allocated();
          for( int j = 0 ; j < need.size() ; ++j){
            if(need[j] > work[j])
              possible = false;
          }
          if(finish[i] == 0 && possible){
            work = work + customers[i]->allocated();
            finish[i] = 1;
          }
        }
        if(finish.is_k(0)){
          check = false;
        }
      }
      if(flag){
        c1[id]->dealloc_req(req);
      }
      return check;
  }

  bool req_approved(int id, const ext_vector<int>& req) {
    if (req.operator>(avail)) { return false; }
    const Customer* c = customers[id];
    if (c->needs_exceeded(req)) { return false; }
    return is_safe(id, req);
  }
  
  void add_customer(Customer* c) {
    customers.push_back(c);
    avail -= c->allocated();
  }
  
  void withdraw_resources(const ext_vector<int>& req) {
    pthread_mutex_lock(&mutex_);
    if (!is_avail(req)) {
      std::cerr << "WARNING: req: " << req << " is not available for withdrawing\n";
      return;
    }
    if (is_avail(req)) { avail -= req; }
    pthread_mutex_unlock(&mutex_);
  }
  void deposit_resources(const ext_vector<int>& req) { avail += req; }


  ext_vector<Customer*> get_customers() const { return customers; }
  
  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "avail: [" << avail << " ]\n";
    
    
    for (Customer* c : customers) {
      c->show();
    }
    
    std::cout << "\n";
    pthread_mutex_unlock(&mutex_);
  }

  friend std::ostream& operator<<(std::ostream& os, const Bank& be) {
    be.show();
    return os;
  }

private:
  ext_vector<int> avail;
  ext_vector<Customer*> customers;
};

#endif /* Bank_h */
