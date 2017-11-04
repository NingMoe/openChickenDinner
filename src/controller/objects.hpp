#ifndef ocd_objects
#define ocd_objects
#include "octree.hpp"
#include "terrain.hpp"
#include "rwmutex.hpp"
class Objects{
  typedef vec3<double> vec;
  octree::octree oct;
  RWMutex locker;
  public:
  int x,y,z;
  Terrain * ter;
  struct node{
    octree::object obj;         //在八叉树中的节点
    node(vec p){
      obj.value=this;
      obj.onfree=[](octree::object * self){
        delete (node*)(self->value);
      };
      this->obj.position=p;
    }
  };
  virtual node* add(vec p){
    locker.Wlock();
    auto r=new node(p);
    oct.insert(&((r)->obj));
    locker.unlock();
    return r;
  }
  virtual void addrandom(void(*callback)(node*,void*),void * arg,int num){
    locker.Wlock();
    for(int i=0;i<num;i++){
      vec p(
        ter->noise.randint(i,0,0)%x,
        ter->noise.randint(i,1,0)%y,
        ter->noise.randint(i,2,0)%z
      );
      auto r=new node(p);
      callback(r,arg);
      oct.insert(&((r)->obj));
    }
    locker.unlock();
  }
  virtual void find(void(*callback)(node*,void*),vec & p,double len,void * arg){
      locker.Rlock();
      struct of_t{
        void(*callback)(node*,void*);
        void * arg;
      }ot;
      ot.callback=callback;
      ot.arg=arg;
      oct.find([](octree::object * obj,void * arg){
          auto self=(of_t*)arg;
          auto re=(node*)obj->value;
          self->callback(re,self->arg);
        },
        p-octree::vec(len,len,len),
        p+octree::vec(len,len,len),
        &ot
      );
      locker.unlock();
  }
};
#endif