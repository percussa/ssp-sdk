#include "Percussa.h"

class TestPluginEditorInterface : public Percussa::SSP::PluginEditorInterface {
public:
    ~TestPluginEditorInterface() {}

    void frameStart() override {}

    void visibilityChanged(bool b) override {}

    void renderToImage(unsigned char *buffer, int width, int height) override {}
};

class TestPluginInterface : public Percussa::SSP::PluginInterface {
public:
    TestPluginInterface() : editor_(nullptr) {

    }

    ~TestPluginInterface() {}

    Percussa::SSP::PluginEditorInterface *getEditor() override {
        if (editor_ == nullptr) {
            editor_ = new TestPluginEditorInterface();
        }
        return editor_;
    }

    void buttonPressed(int n, bool val) override {}

    void encoderPressed(int n, bool val) override {}

    void encoderTurned(int n, int val) override {}

    void inputEnabled(int n, bool val) override {}

    void outputEnabled(int n, bool val) override {}

    void getState(void **buffer, size_t *size) override {}

    void setState(void *buffer, size_t size) override {}

    void prepare(double sampleRate, int samplesPerBlock) override {}

    void process(float **channelData, int numChannels, int numSamples) override {

    }

private:
    Percussa::SSP::PluginEditorInterface *editor_ = nullptr;
};


extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
    static std::vector<std::string> inNames = {"In 1", "In 2"};
    static std::vector<std::string> outNames = {"Out 1", "Out 2"};
    auto desc = new Percussa::SSP::PluginDescriptor;
    desc->name = "TEST";
    desc->descriptiveName = "Test";
    desc->manufacturerName = "Perc";
    desc->version = "1.0.1";
    desc->fileOrIdentifier = "TEST";
    desc->uid = 0x54455354;
    desc->inputChannelNames = inNames;
    desc->outputChannelNames = outNames;
    return desc;
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface *createInstance() {
    return new TestPluginInterface();
}

extern "C" __attribute__ ((visibility("default")))
void getApiVersion(unsigned& major, unsigned& minor) {
    major = Percussa::SSP::API_MAJOR_VERSION;
    minor = Percussa::SSP::API_MINOR_VERSION;
}