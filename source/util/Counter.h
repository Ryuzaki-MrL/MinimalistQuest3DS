#ifndef COUNTER_H
#define COUNTER_H

template <typename T>
class Counter {
  protected:
	static int count;
	~Counter() { --count; }

  public:
	Counter() { ++count; }
	Counter(const Counter&) { ++count; }
	inline static int getCount() { return count; }
};

template <typename T>
int Counter<T>::count(0);

#endif /* COUNTER_H */