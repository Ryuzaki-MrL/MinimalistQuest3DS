#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <stdio.h>

class ISerializable {
  public:
	virtual ~ISerializable() {}

	virtual void serialize(FILE*) const = 0;
	virtual void deserialize(FILE*) = 0;
};

#endif /* SERIALIZABLE_H */