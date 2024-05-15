# pragma once
#include <SFML/Audio.hpp>
#include <cmath>
#include <future>
#include <iostream>
#include <vector>

void generateSound(int note);

class SineWave : public sf::SoundStream {
public:
  void start(int note);

private:
  virtual bool onGetData(Chunk &data);
  virtual void onSeek(sf::Time time);

  std::vector<sf::Int16> m_samples;
  std::size_t m_currentSample = 0;
};