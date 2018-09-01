#ifndef CBUFFER_H
#define CBUFFER_H

// Polymorphism without indirection
template <typename T>
class ClassBuffer {
  private:
	char buf[sizeof(T)];

  public:
	inline T* get() { return reinterpret_cast<T*>(buf); }
	inline operator T*() { return get(); }
	inline operator T&() { return *get(); }
};

#endif /* CBUFFER_H */