# include "sound.h"
extern std::vector<std::future<void>> futures;

void SineWave::start(int note) {
    // Convert the note to a frequency (in Hz)
    double frequency = 440.0 * std::pow(2.0, (note - 69) / 12.0);

    // Initialize the sine wave
    m_samples.clear();
    int sampleRate = 44100;
    for (int i = 0; i < sampleRate; i++) {
        // Calculate the fade out factor (from 1 to 0)
        double fadeOutFactor = 1.0 - static_cast<double>(i) / sampleRate;

        // Generate the sine wave with fade out and reduced volume
        m_samples.push_back(0.5 * 32767 * fadeOutFactor *
                                                std::sin(2 * 3.14159265 * frequency * i / sampleRate));
    }

    // Initialize the sound stream
    initialize(1, sampleRate);
}

bool SineWave::onGetData(Chunk &data) {
  // Set the data to play
  data.samples = &m_samples[0];
  data.sampleCount = m_samples.size();
  if (m_samples.empty()) {
    return false;
  }
  return false;  // Return false to indicate that there are no more samples to stream
}

void SineWave::onSeek(sf::Time time) {
  // Calculate the new position in the samples based on the time
  m_currentSample = static_cast<std::size_t>(
      time.asSeconds() * getSampleRate() * getChannelCount());
}


void waitForSoundToFinish(SineWave *sineWave) {
  // Wait until the sine wave has finished playing
  while (sineWave->getStatus() == sf::SoundStream::Playing) {

    sf::sleep(sf::milliseconds(100));
  }

  sineWave->stop();

  // Delete the sine wave
  delete sineWave;
}

void generateSound(int note) {
  // Create a sine wave for the note
  SineWave *sineWave = new SineWave();
  sineWave->start(note);

  // Play the sine wave
  sineWave->play();

  // Run waitForSoundToFinish in a separate thread and store the future
  futures.push_back(
      std::async(std::launch::async, waitForSoundToFinish, sineWave));
}
