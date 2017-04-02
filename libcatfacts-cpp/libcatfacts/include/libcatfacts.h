#ifndef LIBCATFACTS_H
#define LIBCATFACTS_H

#include <string>

using namespace std;

/**
 * This is a dummy class to demonstrate features of the boiler plate.
 */
class CatFact {
	public:

  /**
   * Default constructur for Dummy (does nothing).
   */
  CatFact();
  /**
   * Returns a bool.
   * @return Always True.
   */
  bool doSomething();

  /**
   * Returns a cat fact.
   * @return a cat fact
   */
  string get();
};

#endif
