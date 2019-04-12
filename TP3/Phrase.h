/*
 * Phrase.h
 *
 *  Created on: 2017-04-11
 *      Author: boust_000
 */

#ifndef PHRASE_H_
#define PHRASE_H_

#include <functional>

using namespace std;

class Phrase
{
private:
protected:
  string _original;
  string _ordonnee;
  int _nMot;

public:
  Phrase(void);
  Phrase(string a_original);
  Phrase(const Phrase &a_phrase);
  virtual ~Phrase(void);
  bool operator<(const Phrase &) const;
  bool operator!=(const Phrase &) const;

  int nMot(void) const;
  int nLettre(void) const;
  string original() const { return _original; }
  string ordonnee() const { return _ordonnee; }

  double rougeW_ord(const Phrase &, function<double(double)>) const;
  double distance(const Phrase &) const;

  friend ostream &operator<<(ostream &a_out, const Phrase &a_phrase);
};

#endif /* PHRASE_H_ */
